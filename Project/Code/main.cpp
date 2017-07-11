#ifndef _MAIN_C_
#define _MAIN_C_

#include <time.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
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


int main( int argc, char *argv[] ) {

//******************************************************************************
//                          CHECK INPUT PARAMETERS
//******************************************************************************
    std::ifstream File;
    try {

          // CHECK if file containing input data exists
          File.open( argv[ 1 ] );
          if ( !File ) {
                std::string ERROR = "ERROR: file Input.data does not exist";
                throw( ERROR );
          }
          File.close();


          // CHECK if file containing Lattice coordinates exists
          File.open( "./Mesh/Coordinates.crd" );
          if ( !File ) {
                std::string ERROR = "ERROR: file /Mesh/Coordinates.crd does not exist";
                throw( ERROR );
          }
          File.close();


          // CHECK if file containing Flag Field exists
          File.open( "./Mesh/FlagFields.fg" );
          if ( !File ) {
                std::string ERROR = "ERROR: file /Mesh/FlagFields.fg does not exist";
                throw( ERROR );
          }
          File.close();


          // CHECK if file containing Lattice Neighbors exists
          File.open( "./Mesh/Neighbors.nb" );
          if ( !File ) {
                std::string ERROR = "ERROR: file /Mesh/Neighbors.nb does not exist";
                throw( ERROR );
          }
          File.close();


          // CHECK if file containing Lattice Neighbors exists
          File.open( "./Mesh/BoundaryList.bc" );
          if ( !File ) {
                std::string ERROR = "ERROR: file /Mesh/BoundaryList.bc does not exist";
                throw( ERROR );
          }
          File.close();

          // CHECK if file containing partitioning exists
          File.open( "./Mesh/CpuPartitioning.prt" );
          if ( !File ) {
                std::string ERROR = "ERROR: file /Mesh/BoundaryList.bc does not exist";
                throw( ERROR );
          }
          File.close();


    }
    catch ( std::string ERROR ) {
        std::cout << ERROR << std::endl;
        File.close();
        return -1;
    }
    catch ( ... ) {
        std::cout << "Unexpected error" << std::endl;
        File.close();
        return -1;
    }

    int RANK = 0;
    int NUMBER_OF_COMMUNICATIONS = 0;

    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &NUMBER_OF_COMMUNICATIONS);
    MPI_Comm_rank (MPI_COMM_WORLD, &RANK);

    const int NUMBER_OF_CPUs = NUMBER_OF_COMMUNICATIONS;

    std::vector< BoundaryBuffer > CommunicationBuffers;
    CommunicationBuffers.resize( NUMBER_OF_COMMUNICATIONS );


//******************************************************************************
//                          INIT PARAMETERS
//******************************************************************************
    // allocate a list for all fluid
    std::vector<Fluid*> FluidDomain;

    // allcocate the list of boundary layer cells
    std::list<BoundaryFluid*> BoundaryList;

    // allocate a list for VTK represenation
    std::vector<Fluid*> VTKrepresentation;

    std::vector<BoundaryEntry*> BoundaryConditions;

    std::unordered_map<unsigned, unsigned> LocalToGlobalIdTable;
    std::unordered_map<unsigned, unsigned> GlobalToLocalIdTable;

    const char* INPUT_FILE_NAME = argv[ 1 ];
    const char* OUTPUT_FILE_NAME = "./Frames/RESULT";
    double Tau = 0.0;
    unsigned TimeSteps = 0;
    unsigned TimeStepsPerPlotting = 0;

    double *collideField = 0;
    double *streamField = 0;
    int *flagField = 0;
    int *CpuID = 0;
    int *VtkID = 0;


    try {
        read_parameters( INPUT_FILE_NAME,         // the name of the data file
                         &Tau,                    // relaxation time
                         &TimeSteps,              // number of simulation time steps
                         &TimeStepsPerPlotting,   // number of visualization time steps
                         RANK );

        initialiseData( &collideField,
                        &streamField,
                        &flagField,
                        &CpuID,
                        &VtkID,
                        FluidDomain,
                        BoundaryConditions,
                        LocalToGlobalIdTable,
                        GlobalToLocalIdTable,
                        RANK,
                        NUMBER_OF_CPUs );

    }
    catch( std::string ERROR ) {
        if ( RANK == MASTER_CPU ) {
            std::cout << ERROR << std::endl;
        }

        MPI_Finalize();
        return - 1;
    }
    catch( ... ) {
        if ( RANK == MASTER_CPU ) {
            std::cout << "Unexpected error" << std::endl;
        }

        MPI_Finalize();
        return - 1;
    }


    scanBoundary(  BoundaryList,
				   FluidDomain,
				   VTKrepresentation,
                   flagField,
				   VtkID,
                   BoundaryConditions,
                   CpuID,
                   RANK,
                   LocalToGlobalIdTable,
                   CommunicationBuffers );


    // remove empty buffers
    // compete initialization of buffers by assigning collideField,
    // the mapping tabble and generate the protocol
    int Iterator = 0;
    unsigned BufferSize = 0;
    while ( Iterator != NUMBER_OF_COMMUNICATIONS ) {

        BufferSize = CommunicationBuffers[ Iterator ].getProtocolSize();

        if ( BufferSize  == 0 ) {
            CommunicationBuffers.erase( CommunicationBuffers.begin() + Iterator );
            --NUMBER_OF_COMMUNICATIONS;
        }
        else {
            CommunicationBuffers[ Iterator ].setField( collideField );
            CommunicationBuffers[ Iterator ].setMappingTable( GlobalToLocalIdTable );
            CommunicationBuffers[ Iterator ].generateProtocol( LocalToGlobalIdTable );
            ++Iterator;
        }
    }



//******************************************************************************
//                          PERFORM COMPUTATION
//******************************************************************************

    MPI_Status STATUS;
    double* Swap = NULL;
    int TAG = 1;
    clock_t Begin = clock();
    for ( unsigned Step = 1; Step <= TimeSteps; ++Step ) {

        for ( unsigned i = 0; i < CommunicationBuffers.size(); ++i ) {

            // Send-receive operation ( the communication subsystem)  takes care 
            // of the issue of preventring cyclic dependencies
            MPI_Sendrecv( CommunicationBuffers[ i ].getProtocol(),
                          CommunicationBuffers[ i ].getProtocolSize(),
                          MPI_DOUBLE,
                          CommunicationBuffers[ i ].getTragetCpu(),
                          TAG,
                          CommunicationBuffers[ i ].getReceiveBuffer(),
                          CommunicationBuffers[ i ].getProtocolSize(),
                          MPI_DOUBLE,
                          CommunicationBuffers[ i ].getTragetCpu(),
                          TAG,
                          MPI_COMM_WORLD,
                          &STATUS );

            CommunicationBuffers[ i ].unpackReceiveBuffer( collideField );
        }


        doStreaming( collideField,
                     streamField,
                     FluidDomain );

        Swap = collideField;
        collideField = streamField;
        streamField = Swap;
    	Swap = NULL;

        doCollision( FluidDomain,
					 collideField,
                     &Tau);

        treatBoundary( collideField,
                       BoundaryList );


#ifndef MLUPS_FLAG
        // if the MLUPS_FLAG flag is enabled then we will measure performance
        // without performing redundant I/O operations

        if ( ( Step % TimeStepsPerPlotting ) == 0 ) {

            writeVtkOutput( OUTPUT_FILE_NAME,
                            RANK,
                            collideField,
                            VtkID,
                            FluidDomain,
                            VTKrepresentation,
                            Step );
        }
#endif


    }


    // display the output information
    clock_t End = clock();

//******************************************************************************
//                          COMPUTE PERFORMANCE
//******************************************************************************
    double Metrics[ 2 ] = { 0.0, 0.0 };

    // Metrics[ 0 ] contains MLUPS
    double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;
    Metrics[ 0 ] = ( FluidDomain.size() * TimeSteps ) / ( ConsumedTime * 1e6 );
    Metrics[ 1 ] = double( FluidDomain.size() );

    double TotalMetrics[ 2 ] = { 0.0, 0.0 };
    MPI_Reduce( Metrics,
                TotalMetrics,
                2,
                MPI_DOUBLE,
                MPI_SUM,
                MASTER_CPU,
                MPI_COMM_WORLD );

    if ( RANK == MASTER_CPU ) {

        // display MLUPS number that stands for Mega Lattice Updates Per Second
        std::cout << "Computational time: " <<  ConsumedTime << " sec" << std::endl;
        std::cout << "MLUPS: " <<  TotalMetrics[ 0 ] << std::endl;
        std::cout << "Number of lattices: " <<  (int)TotalMetrics[ 1 ] << std::endl;

    }

//******************************************************************************
//                          RELEASE RESOURCES
//******************************************************************************

    // delete list of oCoordinatesbstacles
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


    for ( unsigned i = 0; i < BoundaryConditions.size(); ++i ) {
        delete BoundaryConditions[ i ];
    }


    delete [] collideField;
    delete [] streamField;
    delete [] flagField;
    delete [] CpuID;
    delete [] VtkID;

    MPI_Finalize();
    return 0;
}

#endif
