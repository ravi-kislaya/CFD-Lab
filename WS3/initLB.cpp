#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>

//TODO: DO final optimization for variables in for loop

//##############################################################################
// 					Initialize Stream and Collide Fields
//##############################################################################
void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   int *IdField,
					   unsigned *Length ) {

//..............................................................................
// 					Initialize Stream and Collide Fields
//..............................................................................

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


 //..............................................................................

// 						Initialize Flag and ID Fields
//..............................................................................

	// initialize fluid field
	// IMPORTANT: Flag and ID Fields have the same index pattern.
	// it means that we can use computeFlagIndex(...) to compute the index
	// for the ID Field
	const int DEFAULT_ID = -1;
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

				flagField[ computeFlagIndex( x, y, z, Length ) ] = FLUID;
				IdField[ computeFlagIndex( x, y, z, Length ) ] = DEFAULT_ID;

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

			flagField[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length) ] = OUTFLOW;
		}
	}


	// initialize inlet
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

			flagField[ computeFlagIndex( x, y, 0, Length) ] = INFLOW;
		}
	}


	// initialised walls
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = WALL;
			flagField[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = WALL;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, 0, z, Length ) ] = WALL;
			flagField[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = WALL;
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
}




void initialiseFields_LidDtivenCavity( double *collideField,
                                       double *streamField,
                                       int *flagField,
                                       int *IdField,
                                       unsigned* Length ) {

//..............................................................................
// 					Initialize Stream and Collide Fields
//..............................................................................

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


//..............................................................................
// 						Initialize Flag and ID Fields
//..............................................................................

// initialize fluid field
// IMPORTANT: Flag and ID Fields have the same index pattern.
// it means that we can use computeFlagIndex(...) to compute the index
// for the ID Field
	const int DEFAULT_ID = -1;
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

					flagField[ computeFlagIndex( x, y, z, Length ) ] = FLUID;
					IdField[ computeFlagIndex( x, y, z, Length ) ] = DEFAULT_ID;

			}
		}
	}

	// initialised walls: ZY plaту
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = WALL;
			flagField[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = WALL;
		}
	}


	// initialised walls: XZ plaту
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, 0, z, Length ) ] = WALL;
			flagField[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = WALL;
		}
	}


	// initialised walls: YX plaту
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, y, 0, Length ) ] = WALL;
			flagField[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = MOVING_WALL;
		}
	}


}
