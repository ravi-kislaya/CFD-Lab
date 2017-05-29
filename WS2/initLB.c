#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>


int read_parameters( const char *INPUT_FILE_NAME,        /* the name of the data file */
                     unsigned *xlength,                  /* number of cells along x direction */
                     double *tau,                        /* relaxation time */
                     double *U,                          /* lid velocity x-direction */
                     double *V,                          /* lid velocity y-direction */
                     double *W,                          /* lid velocity z-direction */
                     unsigned *timesteps,                     /* number of simulation time steps */
                     unsigned *timestepsPerPlotting ) {       /* number of visualization time steps */

   read_unsigned( INPUT_FILE_NAME, "xlength", xlength );
   read_double( INPUT_FILE_NAME, "tau", tau );

   read_double( INPUT_FILE_NAME, "U", U );
   read_double( INPUT_FILE_NAME, "V", V );
   read_double( INPUT_FILE_NAME, "W", W );

   read_unsigned( INPUT_FILE_NAME, "timesteps", timesteps );
   read_unsigned( INPUT_FILE_NAME, "timestepsPerPlotting", timestepsPerPlotting );

   return 1;
}


void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   unsigned xlength ) {


	//Variable declaration
	unsigned Current_Field_Cell = 0;
	unsigned Current_Flag_Cell = 0;
	unsigned TotalLength = xlength + 2;


   //Initialization of collideField
	for( unsigned z = 0 ; z <= xlength + 1; ++z )  {
		for( unsigned y = 0 ; y <= xlength + 1 ; ++y )  {
			for( unsigned x = 0 ; x <= xlength + 1 ; ++x ) {

				Current_Field_Cell = computeFieldIndex( x, y, z, TotalLength );

		        for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
					//Initialization of collideField
		          	collideField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

					//Initialization of streamField
				  	streamField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
		        }


				//Initialization of flagField
				Current_Flag_Cell = computeFlagIndex( x, y, z, TotalLength );

				if ( z == ( xlength + 1 ) ) {
					flagField [ Current_Flag_Cell ] = MOVING_WALL;
				}
				else if ( ( x == 0 ) || ( y == 0 ) || ( z == 0 )
					   || ( x == ( xlength + 1 ) ) || y == ( ( xlength + 1 ) ) ) {

					flagField [ Current_Flag_Cell ] = WALL;
				}
				else {
					flagField [ Current_Flag_Cell ] = FLUID ;
				}
			}
		}
	}
}
