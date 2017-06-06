#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>

//TODO: DO final optimization for variables in for loop

void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   unsigned *Length ) {

//##############################################################################
// 					Initialize Stream and Collide Fields
//##############################################################################

  unsigned Current_Field_Cell = 0;

  // init Fields: collide and stream fields
  //Initialization of collideField
  for( unsigned z = 0 ; z <= Length[ 2 ] + 1; ++z )  {
	 for( unsigned y = 0 ; y <= Length[ 1 ] + 1 ; ++y )  {
		 for( unsigned x = 0 ; x <= Length[ 0 ] + 1 ; ++x ) {

			 Current_Field_Cell = computeFieldIndex( x, y, z, Length );

			 for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
				 //Initialization of collideField
				 collideField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

				 //Initialization of streamField
				 streamField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
			 }

		 }
	 }
 }


//##############################################################################
// 							Initialize Flag Field
//##############################################################################
	// initialize fluid field
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

				flagField[ computeFlagIndex( x, y, z, Length ) ] = FLUID;

			}
		}
	}


	//
	//	   Coordintes:
	//
	//	  X |
	//		|   /  Y
	//		|  /
	//		| /
	//		|/
	//		------------ Z
	//
	//


	// initialize outlet
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

			flagField[ computeFieldIndex( x, y, Length[ 2 ] + 1, Length) ] = OUTFLOW;
		}
	}

/*
	// initialize inlet
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

			flagField[ computeFieldIndex( x, y, 0, Length) ] = INFLOW;
		}
	}


	// initialised walls
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			flagField[ computeFieldIndex( 0, y, z, Length ) ] = WALL;
			flagField[ computeFieldIndex( Length[ 0 ] + 1, y, z, Length ) ] = WALL;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFieldIndex( x, 0, z, Length ) ] = WALL;
			flagField[ computeFieldIndex( x, Length[ 1 ] + 1, z, Length ) ] = WALL;
		}
	}



	unsigned HalfLength = 0.5 * Length[ 0 ];

	// initialize step
	for ( unsigned z = 0; z <= HalfLength; ++z ) {
		for ( unsigned y = 0; y <= Length[ 1 ] + 1; ++y ) {
			for ( unsigned x = 0; x <= HalfLength; ++x ) {

				flagField[ computeFlagIndex( x, y, z, Length ) ] = WALL;

			}
		}
	}
*/
}
