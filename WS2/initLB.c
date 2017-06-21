#include "helper.h"
#include "visualLB.h"
#include "computeCellValues.h"
#include <stdio.h>
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>

<<<<<<< HEAD
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


=======

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
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

   read_unsigned( INPUT_FILE_NAME, "timesteps", timesteps );
   read_unsigned( INPUT_FILE_NAME, "timestepsPerPlotting", timestepsPerPlotting );

   return 1;
}


void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   unsigned xlength ) {

<<<<<<< HEAD
  //Initialization of collideField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength + 1 ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength + 1 ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength + 1; ++X_Coordinate ) {
=======
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

	//Variable declaration
	unsigned Current_Field_Cell = 0;
	unsigned Current_Flag_Cell = 0;
	unsigned TotalLength = xlength + 2;


   //Initialization of collideField
	for( unsigned z = 0 ; z <= xlength + 1; ++z )  {
		for( unsigned y = 0 ; y <= xlength + 1 ; ++y )  {
			for( unsigned x = 0 ; x <= xlength + 1 ; ++x ) {

<<<<<<< HEAD
  //Initialization of streamField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength + 1 ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength + 1 ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength + 1  ; ++X_Coordinate ) {
        Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
=======
				Current_Field_Cell = computeFieldIndex( x, y, z, TotalLength );
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

		        for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
					//Initialization of collideField
		          	collideField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

					//Initialization of streamField
				  	streamField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
		        }


<<<<<<< HEAD
	//Initialization of flagField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength + 1 ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength + 1 ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength + 1 ; ++X_Coordinate ) {

					Current_Cell = ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

						//TODO : masking
					if( Z_Coordinate == xlength )
						flagField [Current_Cell] = MOVING_WALL ;
					else if(   X_Coordinate == 0 || X_Coordinate == xlength
							|| Y_Coordinate == 0 || Y_Coordinate == xlength )
						flagField [Current_Cell] = WALL ;
					else
						flagField [Current_Cell] = FLUID ;

=======
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
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343
			}
		}
	}
}
