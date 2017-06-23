#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>

#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"


/*******************************************************************************
*
*  						PROBLEM 1 : LID DRIVEN CAVITY
*
*******************************************************************************/
void initialiseFields_LidDrivenCavity( double **collideField,
                                       double **streamField,
                                       int **flagField,
                                       int **IdField,
                                       unsigned* Length,
									   unsigned RANK,
                                       int* PROC,
									   int* CpuCoordinates ) {

//..............................................................................
// 						Allocate the fields in memory
//..............................................................................

	allocateFields( collideField,
					streamField,
					flagField,
					IdField,
					Length );

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
					(*collideField) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

					//Initialization of streamField
					(*streamField) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

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
				( *flagField )[ computeFlagIndex( x, y, z, Length ) ] = FLUID;
				( *IdField )[ computeFlagIndex( x, y, z, Length ) ] = DEFAULT_ID;
			}
		}
	}

	//initialising parallel boundary for YZ parallel planes
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = PARALLEL_BOUNDARY_X;
			( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = PARALLEL_BOUNDARY_X;
		}
	}


	//initialising parallel boundary for XZ parallel planes
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = PARALLEL_BOUNDARY_Y;
			( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = PARALLEL_BOUNDARY_Y;
		}
	}

	//initialising parallel boundary for XY parallel planes
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = PARALLEL_BOUNDARY_Z;
			( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = PARALLEL_BOUNDARY_Z;
		}
	}

	// initialised No Slip: YZ plate
	if( CpuCoordinates[0] == 0 ){
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y )
				( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
		}
	}

	if( CpuCoordinates[0]  == ( PROC[0] - 1 ) ){
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y )
				( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: XZ plate
	if( CpuCoordinates[1] == 0 ) {
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x )
				( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
		}
	}

	if( CpuCoordinates[1] == ( PROC[1] - 1 ) ) {
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x )
				( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: XY plate
	if( CpuCoordinates[2] == 0 ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x )
				( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = NO_SLIP;
		}
	}

	if( CpuCoordinates[2]  == ( PROC[2] - 1 ) ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x )
				( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = MOVING_WALL;
		}
	}
}
