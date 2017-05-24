#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>


/*
int readParameters(int *xlength, double *tau, double *velocityWall, int *timesteps, int *timestepsPerPlotting, int argc, char *argv[]){
  	//DELETE COMMENT: I don't know why argc is used here instead of being used in main.

	if ( argc == 2 ) {
		const char *szFilename = NULL;
		szFilename = argv[1];
		READ_INT( szFilename, *xlength );
		READ_DOUBLE( szFilename, *tau );
		read_double( szFileName, "velocityWall1", &velocityWall[0] );
		read_double( szFileName, "velocityWall2", &velocityWall[1] );
		read_double( szFileName, "velocityWall3", &velocityWall[2] );
		READ_INT( szFilename, *timesteps );
		READ_INT( szFilename , *timestepsPerPlotting );
	}

	else {
		printf("Error : The input is wrong. Pass an input file as a parameter. \n"); // DELETE: I couldn't think of a reasonable error message
		return -1;
	}

	return 0;
}
*/

void initialiseFields(double *collideField, double *streamField, int *flagField, int xlength){
	/* TODO
  1. masking
  */

	//Variable declaration
	int x = 0 , y = 0 , z = 0 , Vel_Component = 0;
	int Current_Cell = 0.0;


	//all the fields are separately initialised for cache optimisation

  //Initialization of collideField
	for( z = 0 ; z <= xlength + 1; ++z )  {
		for( y = 0 ; y <= xlength + 1 ; ++y )  {
			for( x = 0 ; x <= xlength + 1 ; ++x ) {

				Current_Cell = computeFlagIndex( x, y, z, xlength );

        for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
          collideField [ Current_Cell + Vel_Component ] = LATTICEWEIGHTS[ Vel_Component ] ;
        }

			}
		}
	}

  //Initialization of streamField
	for( z = 0 ; z <= xlength + 1; ++z )  {
		for( y = 0 ; y <= xlength + 1; ++y )  {
			for( x = 0 ; x <= xlength + 1; ++x ) {

        Current_Cell = computeFlagIndex( x, y, z, xlength );

        for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
          streamField [ Current_Cell + Vel_Component ] = LATTICEWEIGHTS[ Vel_Component ] ;
        }

			}
		}
	}


	//Initialization of flagField
	for( z = 0 ; z <= ( xlength + 1 ); ++z )  {
		for( y = 0 ; y <= ( xlength + 1 ); ++y )  {
			for( x = 0 ; x <= ( xlength + 1 ); ++x ) {

				Current_Cell = computeFlagIndex( x, y, z, xlength );

				//TODO : masking
				if( z == ( xlength + 1 ) ) {
                    flagField [ Current_Cell ] = MOVING_WALL;
                }
				else if ( x == 0
                          || y == 0
                          || z == 0
                          || x == ( xlength + 1 )
                          || y == ( xlength + 1 ) ) {

						flagField [ Current_Cell ] = WALL;
                }
				else {
					flagField [ Current_Cell ] = FLUID ;
                }

            }
        }
    }
}
