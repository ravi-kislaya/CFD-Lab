#ifndef _MAIN_C_
#define _MAIN_C_

#include <time.h>
#include <list>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>


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
int main( int argc, char *argv[] ){

//******************************************************************************
//                          CHECK INPUT PARAMETERS
//******************************************************************************
    const unsigned NUMBER_OF_MODES = 4;
    static const char* MODES[ NUMBER_OF_MODES ] = { "-fcavity",
                                                    "-fstep",
                                                    "-fchannel",
                                                    "-fplate" };

    try {
     if ( argc >= 3 ) {

         // check if it's possible to open the data file
         FILE* DataFile;
         DataFile = fopen( argv[1], "r" );

         // Abort execution if it's not possible to open the file
         if ( DataFile == 0 ) {
             std::string Error = "";
             Error = "ERROR: The file comtaining the input data does not exsist\n";
             Error.append( "HINT:   Check the file spelling or its existence and try again" );
             throw Error;
         }

         // check a mode was selected right
         int aWrongModeProvided = true;
         for ( unsigned i = 0; i < NUMBER_OF_MODES; ++i ) {
                aWrongModeProvided *= strcmp( argv[2], MODES[ i ] );
         }

         if ( aWrongModeProvided ) {
             std::string Error = "";
             Error = "ERROR: The provided mode does not exsist\n";
             Error.append( "HINT: Check the mode spelling and try again" );
             throw Error;
         }

         // Check the consistence of vtk files if the plane mode was selected
         if( !strcmp( argv[2], MODES[ NUMBER_OF_MODES - 1  ] ) ) {
             if ( argc == 4 ) {
                 // check if it's possible to open a vtk file
                 FILE* VtkFile;
                 VtkFile = fopen( argv[3], "r" );

                 if ( VtkFile == 0 ) {
                    std::string Error = "";
                    Error = "ERROR: The file containing the mesh does not exsist\n";
                    Error.append( "HINT: Check the file spelling or its existence and try again" );
                     throw Error;
                 }

             }

             else {
                std::string Error = "";
                Error = "ERROR: The number of input parameters is wrong\n";
                Error.append( "Example: ./lbsim cube.dat -fstep" );
                throw Error;
             }
         }

     }
     else {
            // Abort execution if the number of input parameters is wrong
            std::string Error = "";
            Error = "ERROR: The number of input parameters is wrong\n";
            Error.append( "HINT: Look at the README file to find out how to run the program\n" );
            Error.append( "Example: ./lbsim cube.dat -fstep" );
            throw Error;
     }
    }
    catch( std::string Error ) {
        std::cout << Error << std::endl;
        return -1;
    }
    catch ( ... ) {
        std::cout << "Unexpected error" << std::endl;
        return -1;
    }



//******************************************************************************
//                          PERFORM COMPUTATION
//******************************************************************************

  // Define basics variables
  const char* INPUT_FILE_NAME = argv[1];

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
  
  // Boundary Buffer Scheme:
  // index 0: +x direction
  // index 1: -x direction
  // index 2: +y direction
  // index 3: -y direction
  // index 4: +z direction
  // index 5: -z direction
  BoundaryBuffer* BoundaryBufferArray[ MAX_COMMUNICATION_FACES ];


  try {

      read_parameters( INPUT_FILE_NAME,         /* the name of the data file */
                       Length,                  /* number of cells along x direction */
                       &tau,                    /* relaxation time */
                       wallVelocity,            /* lid velocity along all direction*/
                       InletVelocity,           /* Inlet velocity along all direction */
                       &DeltaDensity,           /* density difference */
                       &TimeSteps,              /* number of simulation time steps */
                       &TimeStepsPerPlotting ); /* number of visualization time steps */
  }
  catch( std::string Error ) {
    std::cout << Error << std::endl;
    return - 1;
  }
  catch( ... ) {
    std::cout << "Unexpected error" << std::endl;
    return - 1;
  }



      // Lid driven cavity mode
      if ( !strcmp( argv[ 2 ], MODES[ 0 ] ) ) {

          initialiseFields_LidDrivenCavity( &collideField,
                                            &streamField,
                                            &flagField,
                                            &IdField,
                                            Length, 13,3,3,2 );

      }

      // Flow over a step mode
     /* else if ( !strcmp( argv[ 2 ], MODES[ 1 ] ) ) {

          if ( Length[ 0 ] >= Length[ 2 ] ) {
            std::cout << "ERROR: zLength cannot be less or equal then xLength "
                      << "in the fstep mode" << std::endl;
            return -1;
          }

          initialiseFields_Step( &collideField,
                                 &streamField,
                                 &flagField,
                                 &IdField,
                                 Length );

      }

      // Plane Shear Flow mode
      else if( !strcmp( argv[ 2 ], MODES[ 2 ] ) ) {

          initialiseFields_PlaneShearFlow( &collideField,
                                            &streamField,
                                            &flagField,
                                            &IdField,
                                            Length );


      }

      // Tilted Plate mode
      else if( !strcmp( argv[ 2 ], MODES[ 3 ] ) ) {



          initialiseFields_TiltedPlate( argv[ 3 ],
                                        &collideField,
          			                    &streamField,
          			                    &flagField,
          			                    &IdField,
          			                    Length );

      }*/

    // allcocate the list of boundary layer cells
    std::list<BoundaryFluid*> BoundaryList;

    // allocate a list for all fluid
    std::list<Fluid*> FluidDomainList;

    // allocate a list for VTK represenation
    std::list<Fluid*> VTKrepresentation;
	
	


      // prepare all boundaries ( stationary and moving walls )
      scanBoundary( BoundaryList,
    				FluidDomainList,
                    VTKrepresentation,
                    flagField,
                    IdField,
                    Length,
                    wallVelocity,
    				InletVelocity,
    				DeltaDensity );


       std::vector<Fluid*> FluidDomain;
       copyToVector( FluidDomainList, FluidDomain );


       clock_t Begin = clock();
		writeVtkOutput( OUTPUT_FILE_NAME,
                                flagField,
                                FluidDomain,
                                VTKrepresentation,
                                IdField,
                                0,
                                Length );

        // Perform LB method
       /* double* Swap = NULL;
        for ( unsigned Step = 0; Step < TimeSteps; ++Step ) {


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
                                flagField,
                                FluidDomain,
                                VTKrepresentation,
                                IdField,
                                Step,
                                Length );
            }
#endif


        }*/


    // display the output information
    clock_t End = clock();
    double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;
    double MLUPS = ( FluidDomain.size() * TimeSteps ) / ( ConsumedTime * 1e6 );

    // display MLUPS number that stands for Mega Lattice Updates Per Second
    std::cout << "Computational time: " <<  ConsumedTime << " sec" << std::endl;
    std::cout << "MLUPS: " <<  MLUPS << std::endl;
    std::cout << "Number of lattices: " <<  (int)FluidDomain.size() << std::endl;



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
    free( collideField );
    free( streamField );
    free( flagField );
  return 0;
}

#endif
