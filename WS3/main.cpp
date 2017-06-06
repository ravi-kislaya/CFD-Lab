#ifndef _MAIN_C_
#define _MAIN_C_

#include <time.h>
#include <list>
#include <iostream>
#include <stdio.h>


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

int main( int argc, char *argv[] ){


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

             return -1;
         }

     }
     else {

         // Abort execution if the number of input parameters is wrong
         printf( "\nERROR: The number of input parameters is wrong\n\n" );

         printf( "HINT:   You've probably forgot to pass the input file" );
         printf( " as a parameter\n" );
         printf( "\tor you've passed more than one parameter\n\n" );

         printf( "Example: ./sim cavity100.dat\n\n" );
         return -1;
     }


  // Allocate all fields
  const char* INPUT_FILE_NAME = argv[1];
  const char* OUTPUT_FILE_NAME = "./Frames/Cube3D";
  unsigned Length[ 3 ] = { 10, 10, 10 }; //TODO: change this to array
  double tau = 0.0;
  double wallVelocity[ 3 ] = { 0.0, 0.0, 0.0 };
  unsigned TimeSteps = 0;
  unsigned TimeStepsPerPlotting = 0;

  double InletVelocity[ 3 ] = { 0.0, 0.0, 0.0 }; //TODO: Delcare this in read parameter as array
  double DeltaDensity = 0.0; //TODO: Declare this in read parameter as variables



    read_parameters( INPUT_FILE_NAME,           /* the name of the data file */
                       Length,                 /* number of cells along x direction */
                       &tau,                    /* relaxation time */
                       wallVelocity,            /* lid velocity along all direction*/
                       InletVelocity,           /* Inlet velocity along all direction */
                       &DeltaDensity,           /* density difference */
                       &TimeSteps,              /* number of simulation time steps */
                       &TimeStepsPerPlotting ); /* number of visualization time steps */


  // initialize all variables and fields
  int CellNumber = ( Length[ 0 ] + 2 )
                 * ( Length[ 1 ] + 2 )
                 * ( Length[ 2 ] + 2 );

  double *collideField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  double *streamField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  int *flagField = ( int* )calloc( CellNumber, sizeof(int) );
  int *IdField = ( int* )calloc( CellNumber, sizeof( int ) );


  // initialize all fields
/*
  initialiseFields( collideField,
                    streamField,
                    flagField,
                    IdField,
                    Length );
*/

initialiseFields_LidDtivenCavity( collideField,
                                  streamField,
                                  flagField,
                                  IdField,
                                  Length );

  // allcocate the list of boundary layer cells
  std::list<BoundaryFluid*> BoundaryList;

  // allocate a list for all fluid
  std::list<Fluid*> FluidDomain;

  // allocate a list for VTK represenation
  std::list<Fluid*> VTKrepresentation;

  // prepare all boundaries ( stationary and moving walls )
  scanBoundary( BoundaryList,
				FluidDomain,
                VTKrepresentation,
                flagField,
                IdField,
                Length,
                wallVelocity,
				InletVelocity,
				DeltaDensity );


   clock_t Begin = clock();


    // Perform LB method
    double* Swap = NULL;
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
                            collideField,
                            FluidDomain,
                            VTKrepresentation,
                            IdField,
                            Step,
                            Length );
        }
#endif



    }




   // display the output information
   clock_t End = clock();
   double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;
   double MLUPS = ( CellNumber * TimeSteps ) / ( ConsumedTime * 1e6 );

   // display MLUPS number that stands for Mega Lattice Updates Per Second
   printf( "Computational time: %4.6f sec\n",  ConsumedTime );
   printf( "MLUPS: %4.6f\n", MLUPS );
   printf( "Mesh size: %d x %d x %d\n\n",  Length[ 0 ], Length[ 1 ], Length[ 2 ] );


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
	for ( std::list<Fluid*>::iterator Iterator = FluidDomain.begin();
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
