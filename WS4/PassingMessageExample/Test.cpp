#include <iostream>
#include <stdlib.h>
#include "../LBDefinitions.h"
#include <mpi.h>
#include <vector>
#include <list>

unsigned computeCPUCoordinateX( unsigned* Proc,
                                unsigned Rank ) {

	return ( unsigned ) ( Rank % Proc[0] );

}

unsigned computeCPUCoordinateY( unsigned* Proc,
                                unsigned Rank ) {

	return ( ( ( unsigned ) ( Rank / Proc[0] ) ) % Proc[1] );
}


unsigned computeCPUCoordinateZ( unsigned* Proc,
                                unsigned Rank ) {

	return ( unsigned ) ( Rank / ( Proc[1] * Proc[2] ));
}

unsigned getGlobalIndex( unsigned i, unsigned j, unsigned k, unsigned* Proc ) {
    return k * Proc[ 0 ] * Proc[ 1 ] + j * Proc[ 0 ] + i;
};


//******************************************************************************
//                                MAIN
//******************************************************************************
int main( int argc, char** argv ) {
    unsigned Proc[ 3 ] = { atoi(argv[1]), atoi(argv[1]), atoi(argv[3]) };

    int NUMBER_OF_PROCESSORS = 0;
    int RANK = 0;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &NUMBER_OF_PROCESSORS);
    MPI_Comm_rank (MPI_COMM_WORLD, &RANK);
    MPI_Request Request;

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


    // FILTERING
    const unsigned EMPTY_NEIGHBOR = -1;
    if ( ProcX == ( Proc[ 0 ] - 1 ) ) Neighbours[ 0 ] = EMPTY_NEIGHBOR;
    if ( ProcX == 0 ) Neighbours[ 1 ] = EMPTY_NEIGHBOR;

    if ( ProcY == ( Proc[ 1 ] - 1 ) ) Neighbours[ 2 ] = EMPTY_NEIGHBOR;
    if ( ProcY == 0 ) Neighbours[ 3 ] = EMPTY_NEIGHBOR;

    if ( ProcZ == ( Proc[ 2 ] - 1 ) ) Neighbours[ 4 ] = EMPTY_NEIGHBOR;
    if ( ProcZ == 0 ) Neighbours[ 5 ] = EMPTY_NEIGHBOR;



    std::list<int> NeighboursList;
    for ( int i = 0; i < 6; ++i ) {
        if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
            NeighboursList.push_back( Neighbours[ i ] );
        }
    }


    // prepare all data to be transfered
    int SEND_DATA = RANK;
    int RECEIVE_DATA = -1;
    int TAG = 1;

    // sending data to all directions
    for ( int i = 0; i < 6;i += 2) {
        if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
          MPI_Send( &SEND_DATA,
                     1,
                     MPI_INT,
                     Neighbours[ i ],
                     TAG,
                     MPI_COMM_WORLD );
        }

        if ( Neighbours[ i + 1 ] != EMPTY_NEIGHBOR ) {
         MPI_Recv( &RECEIVE_DATA,
                    1,
                    MPI_INT,
                    Neighbours[ i + 1 ],
                    TAG,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE );

          MPI_Send( &SEND_DATA,
                     1,
                     MPI_INT,
                     Neighbours[ i + 1 ],
                     TAG,
                     MPI_COMM_WORLD );
        }

        if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
         MPI_Recv( &RECEIVE_DATA,
                    1,
                    MPI_INT,
                    Neighbours[ i ],
                    TAG,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE );
         }

    }


    if ( Neighbours[ 1 ] != EMPTY_NEIGHBOR ) {

    }

    std::cout << "GOTTEN INFO: " << RECEIVE_DATA << std::endl;



    // transfer and receive all data

    // TODO: RAVIL - put guards at this place

/*
    for ( unsigned i = 0; i < 10; ++i ) {

        std::cout << "Rank " << i << "   "
                  << "ProcX: " << computeCPUCoordinateX( iProc, jProc, kProc, i ) << " "
                  << "ProcY: " << computeCPUCoordinateY( iProc, jProc, kProc, i ) << " "
                  << "ProcZ: " << computeCPUCoordinateZ( iProc, jProc, kProc, i ) << " "
                  << std::endl;
    }
*/
    MPI_Finalize();
    return 0;
}
