#include <iostream>
#include <stdlib.h>
#include "../LBDefinitions.h"
#include <mpi.h>
#include <vector>

unsigned computeCPUCoordinateX( unsigned* Proc,
                                unsigned Rank ) {

 //   return ( unsigned ) ( Rank % Proc[0] );
    return ( unsigned ) ( ( Rank % ( Proc[0] * Proc[1] ) ) % Proc[ 0 ] );
}

unsigned computeCPUCoordinateY( unsigned* Proc,
                                unsigned Rank ) {

    //return ( ( ( unsigned ) ( Rank / Proc[0] ) ) % Proc[1] );
    return ( unsigned ) ( ( Rank % ( Proc[0] * Proc[1] ) ) / Proc[ 0 ] );
}

unsigned computeCPUCoordinateZ( unsigned* Proc,
                                unsigned Rank ) {

	return ( unsigned ) ( Rank / ( Proc[0] * Proc[1] ));
}

unsigned getGlobalIndex( unsigned i, unsigned j, unsigned k, unsigned* Proc ) {
    return k * Proc[ 0 ] * Proc[ 1 ] + j * Proc[ 0 ] + i;
};


//******************************************************************************
//                                MAIN
//******************************************************************************
// EXAMPLE to run: mpirun -N 27 ./LoadExemp 3 3 3 1000000
int main( int argc, char** argv ) {
    unsigned Proc[ 3 ] = { atoi(argv[1]), atoi(argv[2]), atoi(argv[3]) };

    MPI_Status STATUS;
    int NUMBER_OF_PROCESSORS = 0;
    int RANK = 0;

    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &NUMBER_OF_PROCESSORS);
    MPI_Comm_rank (MPI_COMM_WORLD, &RANK);


    // Guards: check is the number of processors are consistent with 
    // the  given layout
    int LayoutProcessorRequest = Proc[ 0 ] * Proc[ 1 ] * Proc[ 2 ];
    if ( NUMBER_OF_PROCESSORS != LayoutProcessorRequest ) {

        if ( RANK == 0 ) {
        std::cout << "ERROR: the number of given processors doesn't\n" 
                  << "match the provided layout topology" << std::endl;
        }
        MPI_Finalize();
        return 0;
    }   


    if ( RANK == 0 ) {
        std::cout << "The total number of CPU's: "<< NUMBER_OF_PROCESSORS << std::endl;
    }


    // compute coordinates of each peocessor
    unsigned ProcX = computeCPUCoordinateX( Proc, RANK );
    unsigned ProcY = computeCPUCoordinateY( Proc, RANK );
    unsigned ProcZ = computeCPUCoordinateZ( Proc, RANK );


    // init all neighbours
  	// Boundary Buffer Scheme:
  	// index 0: +x direction
  	// index 1: -x direction
  	// index 2: +y direction
  	// index 3: -y directionProc
  	// index 4: +z direction
  	// index 5: -z direction
    std::vector<int> Neighbours;
    Neighbours.push_back( getGlobalIndex( ProcX + 1, ProcY, ProcZ, Proc ) );
    Neighbours.push_back( getGlobalIndex( ProcX - 1, ProcY, ProcZ, Proc ) );
    Neighbours.push_back( getGlobalIndex( ProcX, ProcY + 1, ProcZ, Proc ) );
    Neighbours.push_back( getGlobalIndex( ProcX, ProcY - 1, ProcZ, Proc ) );
    Neighbours.push_back( getGlobalIndex( ProcX, ProcY, ProcZ + 1, Proc ) );
    Neighbours.push_back( getGlobalIndex( ProcX, ProcY, ProcZ - 1, Proc ) );


    // filtering all unnecessary
    const unsigned EMPTY_NEIGHBOR = -1;
    if ( ProcX == ( Proc[ 0 ] - 1 ) ) Neighbours[ 0 ] = EMPTY_NEIGHBOR;
    if ( ProcX == 0 ) Neighbours[ 1 ] = EMPTY_NEIGHBOR;

    if ( ProcY == ( Proc[ 1 ] - 1 ) ) Neighbours[ 2 ] = EMPTY_NEIGHBOR;
    if ( ProcY == 0 ) Neighbours[ 3 ] = EMPTY_NEIGHBOR;

    if ( ProcZ == ( Proc[ 2 ] - 1 ) ) Neighbours[ 4 ] = EMPTY_NEIGHBOR;
    if ( ProcZ == 0 ) Neighbours[ 5 ] = EMPTY_NEIGHBOR;


    // prepare all data to be transfered
    int TAG = 1;
    unsigned SIZE = atoi(argv[4]);
    double* LOAD_DATA = new double[ SIZE ];
    double* RECEIVE_DATA = new double[ SIZE ];
    int COUNTER = 0;

    // Init buffers
    for ( int i = 0; i < SIZE; ++i ) {
        LOAD_DATA[ i ] = 666;
        RECEIVE_DATA[ i ] = -1;
    }

    // perform communication
    for (int i = 0; i < 6; i += 2) {

        if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
            MPI_Send( LOAD_DATA,
                      SIZE,
                      MPI_INT,
                      Neighbours[ i ],
                      TAG,
                      MPI_COMM_WORLD);
                                       
        }
                    
        if ( Neighbours[ i + 1 ] != EMPTY_NEIGHBOR ) {
             MPI_Recv( RECEIVE_DATA,
                       SIZE,
                       MPI_INT,
                       Neighbours[ i + 1 ],
                       TAG,
                       MPI_COMM_WORLD,
                       &STATUS );
                    

            MPI_Send( LOAD_DATA,
                      SIZE,
                      MPI_INT,
                      Neighbours[ i + 1 ],
                      TAG,
                      MPI_COMM_WORLD );
        }
             

        if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
                MPI_Recv( RECEIVE_DATA,
                          SIZE,
                          MPI_INT,
                          Neighbours[ i ],
                          TAG,
                          MPI_COMM_WORLD,
                          &STATUS );
        }

                    
    }


    delete [] LOAD_DATA;
    delete [] RECEIVE_DATA;
    
    MPI_Finalize();
    return 0;
}
