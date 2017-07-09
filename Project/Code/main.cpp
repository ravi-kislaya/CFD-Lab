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

    int RANK = std::stoi( argv[ 2 ] );
    int NUMBER_OF_CPUs = 6;
    std::vector< BoundaryBuffer > CommunicationBuffers;
    CommunicationBuffers.resize( NUMBER_OF_CPUs );

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
                         &TimeStepsPerPlotting ); // number of visualization time steps
    }
    catch( std::string Error ) {
        std::cout << Error << std::endl;
        return - 1;
    }
    catch( ... ) {
        std::cout << "Unexpected error" << std::endl;
        return - 1;
    }


    initialiseData( &collideField,
                    &streamField,
                    &flagField,
                    &CpuID,
                    &VtkID,
                    FluidDomain,
                    BoundaryConditions,
                    LocalToGlobalIdTable,
                    GlobalToLocalIdTable,
                    RANK );


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


    // remove empty buffers from communication bufer array
    int Iterator = 0;
    while ( Iterator != NUMBER_OF_CPUs ) {
        if ( CommunicationBuffers[ Iterator ].getBufferSize() == 0 ) {
            CommunicationBuffers.erase( CommunicationBuffers.begin() + Iterator );
            --NUMBER_OF_CPUs;
        }
        else {
            ++Iterator;
        }
    }

    // DEBUGGING
    for ( unsigned i = 0; i < CommunicationBuffers.size(); ++i ) {
        std::cout << "Buffer containes: " << CommunicationBuffers[ i ].getBufferSize() << " "
                  << "TragetCPU: " << CommunicationBuffers[ i ].getTragetCpu() << " "
                  << "Current CPU: " << RANK
                  << std::endl;
    }


    writeVtkOutput( OUTPUT_FILE_NAME,
                    collideField,
                    VtkID,
                    FluidDomain,
                    VTKrepresentation,
                    0 );




//******************************************************************************
//                          PERFORM COMPUTATION
//******************************************************************************


/*
    clock_t Begin = clock();

    // Perform LB method
    double* Swap = NULL;
    for ( unsigned Step = 1; Step <= TimeSteps; ++Step ) {

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
    double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;
    double MLUPS = ( FluidDomain.size() * TimeSteps ) / ( ConsumedTime * 1e6 );

    // display MLUPS number that stands for Mega Lattice Updates Per Second
    std::cout << "Computational time: " <<  ConsumedTime << " sec" << std::endl;
    std::cout << "MLUPS: " <<  MLUPS << std::endl;
    std::cout << "Number of lattices: " <<  (int)FluidDomain.size() << std::endl;
*/


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
	return 0;
}

#endif
