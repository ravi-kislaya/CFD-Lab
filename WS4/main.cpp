#include <time.h>
#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <mpi.h>

#include "LBDefinitions.h"
#include "collision.h"
#include "streaming.h"
#include "initLB.h"
#include "visualLB.h"
#include "boundary.h"
#include "DataStructure.h"
#include "visualLB.h"
#include "computeCellValues.h"
#include "helper.h"

// the following possible simulation flags has to be set up:
//     -fcavity
//     -fstep
//     -fchannel
//     -fplate
int main( int argc, char *argv[] ) {

//******************************************************************************
//                          CHECK INPUT PARAMETERS
//******************************************************************************


     // check the correctness of the input parameters
     //
     // stop the execution if:
     //
     // 1. the number of input parameters is not equal to two
     //    ( including both the program name and the name of the input data file )
     //
     // 2. there is no such file at the project folder
     //
     if ( argc == 2 ) {

         // check if it's possible to open the file
         FILE* file;
         file = fopen( argv[1], "r" );

         // Abort execution if it's not possible to open the file
         if ( file == 0 ) {
             printf( "\nERROR: The file comtaining the input data doesn't exsist\n\n" );
             printf( "HINT:   Check the file spelling or its existence and try again\n\n");
             
             fclose( file );
             return -1;
         }

        fclose( file );
    }
    else {

         // Abort execution if the number of input parameters is wrong
         printf( "\nERROR: The number of input parameters is wrong\n\n" );

         printf( "HINT: You've probably forgot to pass the input file" );
         printf( " as a parameter\n" );
         printf( "\tor you've passed more than one parameter\n\n" );

         printf( "Example: ./lbsim cavity.dat\n\n" );
         
         return -1;
    }


//******************************************************************************
//                          PERFORM COMPUTATION
//******************************************************************************
    int NUMBER_OF_PROCESSORS = 0;
    int RANK = 0;
    MPI_Status STATUS;


    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &NUMBER_OF_PROCESSORS);
    MPI_Comm_rank (MPI_COMM_WORLD, &RANK);

  // Define basics variables
  // MPI SECTION: START
  int PROC[ Dimensions ] = { 0 };
  // MPI SECTION: END

  const char* INPUT_FILE_NAME = argv[ 1 ];

#ifndef MLUPS_FLAG
  const char* OUTPUT_FILE_NAME = "./Frames/RESULT";
#endif

  unsigned Length[ 3 ] = { 0, 0, 0 };
  double tau = 0.0;
  double wallVelocity[ 3 ] = { 0.0, 0.0, 0.0 };
  unsigned TimeSteps = 0;
  unsigned TimeStepsPerPlotting = 0;

  double InletVelocity[ 3 ] = { 0.0, 0.0, 0.0 };
  double DeltaDensity = 0.0;

  double *collideField = 0;
  double *streamField = 0;
  int *flagField = 0;
  int *IdField = 0;


    try{
        read_parameters( INPUT_FILE_NAME,         /* the name of the data file */
                         Length,                  /* number of cells along x direction */
                         PROC,                    /* processor layout */
                         &tau,                    /* relaxation time */
                         wallVelocity,            /* lid velocity along all direction*/
                         InletVelocity,           /* Inlet velocity along all direction */
                         &DeltaDensity,           /* density difference */
                         &TimeSteps,              /* number of simulation time steps */
                         &TimeStepsPerPlotting,   /* number of visualization time steps */
                         RANK );                  /* CPU rank */
    }
    catch ( std::string ERROR ) {
        
        if ( RANK == MASTER_CPU )
            std::cout << ERROR << std::endl;
        
        MPI_Finalize();
        return 0;
    }


    // compute coordinates of each peocessor
    int ProcZ = computeCPUCoordinateZ( PROC, RANK );
    int ProcY = computeCPUCoordinateY( PROC, RANK );
    int ProcX = computeCPUCoordinateX( PROC, RANK );
    int CpuCoordinates[ Dimensions ] = { ProcX, ProcY, ProcZ };


    initialiseFields_LidDrivenCavity( &collideField,
                                      &streamField,
                                      &flagField,
                                      &IdField,
                                      Length,
                                      RANK,
                                      PROC,
                                      CpuCoordinates );


  // Boundary Buffer Scheme:
  // index 0: +x direction
  // index 1: -x direction
  // index 2: +y direction
  // index 3: -y direction
  // index 4: +z direction
  // index 5: -z direction
  BoundaryBuffer BoundaryBufferArray[ MAX_COMMUNICATION_FACES ];

  // init all buffers
  for ( int i = 0; i < MAX_COMMUNICATION_FACES; ++i ) {
    BoundaryBufferArray[ i ].setIndex( i );
    BoundaryBufferArray[ i ].setDomainLength( Length );
    BoundaryBufferArray[ i ].setField( collideField );
  }




    // allcocate the list of boundary layer cells
    std::list<BoundaryFluid*> BoundaryList;

    // allocate a list for all fluid
    std::list<Fluid*> FluidDomainList;

    // allocate a list for VTK represenation
    std::list<Fluid*> VTKrepresentation;




      // prepare all boundaries ( stationary and moving walls )
      scanBoundary( BoundaryList,
    				FluidDomainList,
                    BoundaryBufferArray,
                    VTKrepresentation,
                    flagField,
                    IdField,
                    Length,
                    wallVelocity );

    for ( int i = 0; i < MAX_COMMUNICATION_FACES; ++i ) {
        BoundaryBufferArray[ i ].generateProtocol();
    }


    // init all neighbours
  	// Boundary Buffer Scheme:
  	// index 0: +x direction
  	// index 1: -x direction
  	// index 2: +y direction
  	// index 3: -y directionProc
  	// index 4: +z direction
  	// index 5: -z direction
    std::vector<int> Neighbours;
    Neighbours.push_back( getGlobalCPUIndex( ProcX + 1, ProcY, ProcZ, PROC ) );
    Neighbours.push_back( getGlobalCPUIndex( ProcX - 1, ProcY, ProcZ, PROC ) );
    Neighbours.push_back( getGlobalCPUIndex( ProcX, ProcY + 1, ProcZ, PROC ) );
    Neighbours.push_back( getGlobalCPUIndex( ProcX, ProcY - 1, ProcZ, PROC ) );
    Neighbours.push_back( getGlobalCPUIndex( ProcX, ProcY, ProcZ + 1, PROC ) );
    Neighbours.push_back( getGlobalCPUIndex( ProcX, ProcY, ProcZ - 1, PROC ) );


    // filtering all unnecessary
    const int EMPTY_NEIGHBOR = -1;
    if ( ProcX == ( PROC[ 0 ] - 1 ) ) Neighbours[ 0 ] = EMPTY_NEIGHBOR;
    if ( ProcX == 0 ) Neighbours[ 1 ] = EMPTY_NEIGHBOR;

    if ( ProcY == ( PROC[ 1 ] - 1 ) ) Neighbours[ 2 ] = EMPTY_NEIGHBOR;
    if ( ProcY == 0 ) Neighbours[ 3 ] = EMPTY_NEIGHBOR;

    if ( ProcZ == ( PROC[ 2 ] - 1 ) ) Neighbours[ 4 ] = EMPTY_NEIGHBOR;
    if ( ProcZ == 0 ) Neighbours[ 5 ] = EMPTY_NEIGHBOR;



       std::vector<Fluid*> FluidDomain;
       copyToVector( FluidDomainList, FluidDomain );


       clock_t Begin = clock();


        // Perform LB method
        double* Swap = NULL;
        
        // Define the protocol size of the receiver
        unsigned MaxProtocolSize = 0;
        for ( int i = 0; i < MAX_COMMUNICATION_FACES; ++i ) {
            if ( MaxProtocolSize < BoundaryBufferArray[ i ].getProtocolSize() ) {
                MaxProtocolSize = BoundaryBufferArray[ i ].getProtocolSize();
            }
        }

        double* ReceiveProtocol = new double[ MaxProtocolSize ];
        
        
        
        int TAG = 1;
        for ( unsigned Step = 0; Step < TimeSteps; ++Step ) {

// .......................COMMUNICATION: START......................
            for (int i = 0; i < MAX_COMMUNICATION_FACES; i += 2) {

                if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
                    MPI_Send( BoundaryBufferArray[ i ].getProtocol(),
                              BoundaryBufferArray[ i ].getProtocolSize(),
                              MPI_DOUBLE,
                              Neighbours[ i ],
                              TAG,
                              MPI_COMM_WORLD );

                }

                if ( Neighbours[ i + 1 ] != EMPTY_NEIGHBOR ) {
                     MPI_Recv( ReceiveProtocol,
                               BoundaryBufferArray[ i + 1 ].getProtocolSize(),
                               MPI_DOUBLE,
                               Neighbours[ i + 1 ],
                               TAG,
                               MPI_COMM_WORLD,
                               &STATUS );


                    decodeProtocol( ReceiveProtocol,
                    				BoundaryBufferArray[ i + 1 ].getProtocolSize(),
                    				collideField );          


                    MPI_Send( BoundaryBufferArray[ i + 1 ].getProtocol(),
                              BoundaryBufferArray[ i + 1 ].getProtocolSize(),
                              MPI_DOUBLE,
                              Neighbours[ i + 1 ],
                              TAG,
                              MPI_COMM_WORLD );
                }


                if ( Neighbours[ i ] != EMPTY_NEIGHBOR ) {
                        MPI_Recv( ReceiveProtocol,
                                  BoundaryBufferArray[ i ].getProtocolSize(),
                                  MPI_DOUBLE,
                                  Neighbours[ i ],
                                  TAG,
                                  MPI_COMM_WORLD,
                                  &STATUS );

                    decodeProtocol( ReceiveProtocol,
                    				BoundaryBufferArray[ i ].getProtocolSize(),
                    				collideField );

                  }
            }
            // .......................COMMUNICATION: END........................


            doStreaming( collideField,
                         streamField,
                         FluidDomain );


            Swap = collideField;
            collideField = streamField;
            streamField = Swap;
    		Swap = NULL;


            doCollision( FluidDomain,
    					 collideField,
                         &tau);


            treatBoundary( collideField,
                           BoundaryList,
                           wallVelocity );


#ifndef MLUPS_FLAG
            // if the MLUPS_FLAG flag is enabled then we will measure performance
            // without performing redundant I/O operations


            if ( ( Step % TimeStepsPerPlotting ) == 0 ) {

                writeVtkOutput( OUTPUT_FILE_NAME,
                                RANK,
                                CpuCoordinates,
                                collideField,
                                FluidDomain,
                                VTKrepresentation,
                                IdField,
                                Step,
                                Length );
            }

#endif


        }


    // compute the metric an a local node
    clock_t End = clock();
    double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;
    double MLUPS = ( FluidDomain.size() * TimeSteps ) / ( ConsumedTime * 1e6 );


    // All SLAVE CPU's have to send their metrics to the MASTER CPU
    if ( RANK != MASTER_CPU ) {

            double METRICS[ 2 ] = { MLUPS, (double)FluidDomain.size() };
            MPI_Send( METRICS,
                      2,
                      MPI_DOUBLE,
                      MASTER_CPU,
                      TAG,
                      MPI_COMM_WORLD );

    }

    // Reveive the metrics from all SLAVE CPU's and sum all of them together
    if ( RANK == MASTER_CPU ) {

        // Allocate temporary buffers for both MPLUPS and domain size
        double MlupsBuffer[ NUMBER_OF_PROCESSORS ];
        unsigned DomainSizeBuffer[ NUMBER_OF_PROCESSORS ];

        // initialize the first entries by MASTER CPU data
        MlupsBuffer[ 0 ] = MLUPS;
        DomainSizeBuffer[ 0 ] = (unsigned)FluidDomain.size();


        double TempReceiveBuffer[ 2 ];
        for( int i = 1; i < NUMBER_OF_PROCESSORS; ++i ) {


            MPI_Recv( TempReceiveBuffer,
                      2,
                      MPI_DOUBLE,
                      i,
                      TAG,
                      MPI_COMM_WORLD,
                      &STATUS );


            // assigned received values to the corresponding buffers
            MlupsBuffer[ i ] = TempReceiveBuffer[ 0 ];
            DomainSizeBuffer[ i ] = (unsigned)TempReceiveBuffer[ 1 ];

        }

        // compute the total value of MLUPS and the total domain size
        MLUPS = performArrayReduction( MlupsBuffer, 
                                       unsigned( NUMBER_OF_PROCESSORS ) );

        unsigned TotalDomainSize = performArrayReduction( DomainSizeBuffer, 
                                                          unsigned( NUMBER_OF_PROCESSORS ) );

        // display the metrics
        std::cout << "Computational time: " <<  ConsumedTime << " sec" << std::endl;
        std::cout << "MLUPS: " <<  MLUPS << std::endl;
        std::cout << "Number of lattices: " <<  TotalDomainSize << std::endl;
    }


    // delete list of obstacles
    for ( std::list<BoundaryFluid*>::iterator Iterator = BoundaryList.begin();
          Iterator != BoundaryList.end();
          ++ Iterator ) {
        // clean all obstacles assigned to the fluid cell
        (*Iterator)->deleteObstacles();

        // delete all fluid cells
        delete (*Iterator);

    }

	// delete list of Fluid
	for ( std::vector<Fluid*>::iterator Iterator = FluidDomain.begin();
          Iterator != FluidDomain.end();
          ++ Iterator ) {

        // delete all fluid cells
        delete (*Iterator);

    }


    // delete flields
    delete [] ReceiveProtocol;
    free( collideField );
    free( streamField );
    free( flagField );


    MPI_Finalize();
  return 0;
}
