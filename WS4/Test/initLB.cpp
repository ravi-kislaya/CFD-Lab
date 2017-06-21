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
									   unsigned rank,
									   unsigned iProj,
									   unsigned jProj,
									   unsigned kProj) {

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
	unsigned ProcZ = (int) rank / ( iProj * jProj);
	unsigned Temp = rank % ( iProj * jProj);
	unsigned ProcY = (int) Temp / iProj;
	unsigned ProcX = (int) Temp % iProj;
	printf("%d %d %d\n", ProcX, ProcY, ProcZ);

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
				( *flagField )[ computeFlagIndex( x, y, z, Length ) ] = 0; //Fluid
				( *IdField )[ computeFlagIndex( x, y, z, Length ) ] = 0;
			}
		}
	}
	
	//initialising parallel boundary for YZ parallel planes
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = 50; //Parallel YZ
			( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = 50;//Parallel YZ
		}
	}
	
		
	//initialising parallel boundary for XZ parallel planes
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = 100;//Parallel XZ
			( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = 100;//Parallel XZ
		}
	}
	
	//initialising parallel boundary for XY parallel planes
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = 150;//Parallel XY
			//printf("hj\n");
			( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = 150;
		}
	}

	// initialised No Slip: YZ plate
	if( ProcX == 0 ){
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) 
				( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = 200;//No Slip
		}
	}
	
	if( ProcX % iProj == iProj - 1 ){
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) 
				( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = 200;
		}
	}


	// initialised walls: XZ plate
	if( ProcY == 0 ) {
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) 
				( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = 200;
		}
	}
	
	if( ProcY % jProj == jProj - 1 ) {
		for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) 
				( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = 200;
		}
	}


	// initialised walls: XY plate
	if( ProcZ == 0 ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) 
				( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = 200;
		}
	}
	
	if( ProcZ % kProj == kProj - 1 ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) 
				( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = 250;//Moving Wall
		}
	}
}