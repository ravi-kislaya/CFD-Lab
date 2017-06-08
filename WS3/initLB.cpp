#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <stdio.h>
#include <iostream>

//TODO: DO final optimization for variables in for loop

//##############################################################################
// 					Initialize Stream and Collide Fields
//##############################################################################

/*
*
*  PROBLEM 1 : TiltedPlate
*
*/
void initialiseFields_TiltedPlate( const char *PLATE_TXT_FILE_NAME ,
								   double *collideField ,
								   double *streamField ,
								   int *flagField ,
								   int *IdField ,
								   unsigned* Length ){

	unsigned Current_Field_Cell = 0;
	unsigned Current_Flag_Cell = 0;
	const int DEFAULT_ID = -1;
		
	// init Fields: collide and stream fields
	//Initialization of collideField
	// initialize fluid field
	// IMPORTANT: Flag and ID Fields have the same index pattern.
	// it means that we can use computeFlagIndex(...) to compute the index
	// for the ID Field
		
	for( unsigned z = 0 ; z <= Length[ 2 ] + 1; ++z )  {
		 for( unsigned y = 0 ; y <= Length[ 1 ] + 1 ; ++y )  {
			 for( unsigned x = 0 ; x <= Length[ 0 ] + 1 ; ++x ) {

				Current_Field_Cell = computeFieldIndex( x, y, z, Length );
				Current_Flag_Cell  = computeFlagIndex ( x, y, z, Length );
					
				flagField[ computeFlagIndex( x, y, z, Length ) ] = FLUID;
				IdField[ computeFlagIndex( x, y, z, Length ) ] = DEFAULT_ID;
					
				for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
					//Initialization of collideField
					collideField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

					//Initialization of streamField
					streamField [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
				}

			}
		}
	}

	//
	//	   Coordintes:
	//
	//	X   |
	//		|   /  Y
	//		|  /
	//		| /
	//		|/
	//		------------ Z
	//
	//

	// Boundary Condition for Z = 0  is set to INFLOW and Z = Zmax is set to OUTFLOW
	for (int x = 0; x < Length[0] + 2; ++x) {
		for (int y = 0; y < Length[1] + 2; ++y) {
			flagField[ computeFlagIndex( x, y, 0, Length ) ] = INFLOW; //Left
			flagField[ computeFlagIndex( x, y, Length[2] + 1, Length ) ] = OUTFLOW; //Right
		}
	}
	// Boundary Condition for X = 0 and X = Xmax is set to NO_SLIP
	for (int y = 0; y < Length[1] + 2; ++y) {
		for (int z = 0; z < Length[2] + 2; ++z) {
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP; //Bottom
			flagField[ computeFlagIndex( Length[0] + 1, y, z, Length ) ] = NO_SLIP; //Top
		}
	}
	// Boundary Condition for Y = 0 and Y = Ymax is set to FREE_SLIP
	for (int x = 0; x < Length[0] + 2; ++x) {
		for (int z = 0; z < Length[2] + 2; ++z) {
			flagField[computeFlagIndex(x, 0, z, Length)] = FREE_SLIP; //XZ face
			flagField[computeFlagIndex(x, Length[1] + 1, z, Length)] = FREE_SLIP; //XZ face
		}
	}
	int **TiltedPlate = imatrix( 0, Length[0]+1, 0, Length[2]+1);

	
	TiltedPlate = read_pgm("lbm_tilted_plate.vtk");

	for (int z = 1; z <= Length[2]; ++z) {
		for (int y = 1; y <= Length[1]; ++y) {
			for (int x = 1; x <= Length[0]; ++x) {
				if(TiltedPlate[x][z] == 1){
					printf("Bhai Sahab \n");
					flagField[ computeFlagIndex( x, y, z, Length )] = NO_SLIP;
				}
			}
		}
	}
	
    free_imatrix(TiltedPlate, 0, Length[0]+1, 0, Length[2]+1);
	
		// ***********************************************************************************************************
						/*Ravil's implementation, which I didn't understand
						
						// initialize the obstacle
						std::ifstream file("lbm_tilted_plate.txt");
						std::string str;
						std::list<std::string> List;
						const char DELIMITER = ' ';
						while (std::getline(file, str))
						{
						  //TODO: Consider to avoid computing the full length of the string
							str.erase( std::remove( str.begin(), str.end(), DELIMITER ), str.end() ) ;
							List.push_back(str);
						
						
						}
						file.close();
						
						int Length[ 3 ] = { 0 };
						
						Length[ 0 ] = ( *List.begin() ).size() + 2;
						Length[ 1 ] = 3;
						Length[ 2 ] = List.size() + 2;
						
						cout << "x direction: " << Length[ 0 ] << endl;
						cout << "y direction: " << Length[ 1 ] << endl;
						cout << "z direction: " << Length[ 2 ] << endl;
						
						
						
						unsigned StringCounter = 0;
						unsigned CharCounter = 0;
						for( std::list<std::string>::iterator aString = List.begin();
							 aString != List.end();
							++aString ) {
						
							for ( std::string::iterator it = (*aString).begin();
								  it != (*aString).end(); ++it ) {
													if (*it == 1){
														flagField[ computeFlagIndex( x, y, z, Length ) ] = NO_SLIP;
														++CharCounter;
													}
							}
							CharCounter = 0;
							++StringCounter;
						}
						
						// The values for Boundary Z = 0 set to Inflow and Zmax plane set to Outflow
						for (int x = 0; x <= Length[0] + 2; ++x) {
								for (int y = 0; y <= Length[1] + 2; ++y) {
										flagField[ computeFlagIndex( x, y, 0, Length ) ] = INFLOW; //Left
										flagField[ computeFlagIndex( x, y, Length[0][2] + 2, Length ) ] = OUTFLOW; //Right
								}
						}
						// The values for Boundary on X = 0 and Xmax plane set to NO_SLIP
							for (int y = 0; y <= Length[0][1] + 2; ++y) {
								for (int z = 0; z <= Length[0][2] + 2; ++z) {
									flagField[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP; //Bottom
									flagField[ computeFlagIndex( Length[0] + 2, y, z, Length ) ] = NO_SLIP; //Top
								}
							}
							// The values for Boundary on Y = 0 and Ymax plane set to FREE_SLIP
							for (int x = 0; x <= Length[0][0] + 2; ++x) {
								for (int z = 0; z <= Length[0][2] + 2; ++z) {
									flagField[computeFlagIndex( x, 0, z, Length)] = FREE_SLIP; //XZ face
									flagField[computeFlagIndex( x, Length[0][1] + 2, z, Length )] = FREE_SLIP; //XZ face
								}
							} */

}

/*
*
*  PROBLEM 2 : Plane Shear Flow
*
*/

void initialiseFields_PlaneShearFlow( double *collideField,
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
	//	X |
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

			flagField[ computeFlagIndex( x, y, 0, Length) ] = PRESSURE_IN;
		}
	}


	// initialised walls
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = FREE_SLIP;
			flagField[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = FREE_SLIP;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}

}
/*
*
*  PROBLEM 3 : Step
*
*/
void initialiseFields_Step( double *collideField,
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
	//	X |
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
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}


	unsigned HalfLength = 0.5 * Length[ 0 ];

	// initialize step
	for ( unsigned z = 0; z <= HalfLength; ++z ) {
		for ( unsigned y = 0; y <= Length[ 1 ] + 1; ++y ) {
			for ( unsigned x = 0; x <= HalfLength; ++x ) {

				flagField[ computeFlagIndex( x, y, z, Length ) ] = NO_SLIP;

			}
		}
	}
}




void initialiseFields_LidDrivenCavity( double *collideField,
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
			flagField[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: XZ plaту
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: YX plaту
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			flagField[ computeFlagIndex( x, y, 0, Length ) ] = NO_SLIP;
			flagField[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = MOVING_WALL;
		}
	}


}
