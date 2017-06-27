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
                                       unsigned* Length ) {

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

	// initialised walls: ZY plaту
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: XZ plaту
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}


	// initialised walls: YX plaту
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = MOVING_WALL;
		}
	}
}


/*******************************************************************************
*
*  								PROBLEM 2 : STEP
*
*******************************************************************************/
void initialiseFields_Step( double **collideField,
					   double **streamField,
					   int **flagField,
					   int **IdField,
					   unsigned *Length ) {


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
				 ( *collideField ) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

				 //Initialization of streamField
				 ( *streamField ) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
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

			( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length) ] = OUTFLOW;
		}
	}


	// initialize inlet
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

			( *flagField )[ computeFlagIndex( x, y, 0, Length) ] = INFLOW;
		}
	}


	// initialised walls
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = NO_SLIP;
		}
	}


	unsigned HalfLength = 0.5 * Length[ 0 ];

	// initialize step
	for ( unsigned z = 0; z <= HalfLength; ++z ) {
		for ( unsigned y = 0; y <= Length[ 1 ] + 1; ++y ) {
			for ( unsigned x = 0; x <= HalfLength; ++x ) {

				( *flagField )[ computeFlagIndex( x, y, z, Length ) ] = NO_SLIP;

			}
		}
	}
}


/*******************************************************************************
*
*  						PROBLEM 3: Plane Shear Flow
*
********************************************************************************/

void initialiseFields_PlaneShearFlow( double **collideField,
					   double **streamField,
					   int **flagField,
					   int **IdField,
					   unsigned *Length ) {



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
				 ( *collideField ) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];

				 //Initialization of streamField
				 ( *streamField ) [ Current_Field_Cell + i ] = LATTICEWEIGHTS[ i ];
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

			( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length) ] = OUTFLOW;
		}
	}


	// initialize inlet
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

			( *flagField )[ computeFlagIndex( x, y, 0, Length) ] = PRESSURE_IN;
		}
	}


	// initialised walls
	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
			( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
		}
	}


	for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = FREE_SLIP;
			( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = FREE_SLIP;
		}
	}
}



/*******************************************************************************
*
*  						PROBLEM 4: TiltedPlate
*
********************************************************************************/
void initialiseFields_TiltedPlate( const char *PLATE_TXT_FILE_NAME,
								   double **collideField,
								   double **streamField,
								   int **flagField,
								   int **IdField,
								   unsigned* Length ) {

  // initialize the obstacle
  std::ifstream File( PLATE_TXT_FILE_NAME );

  std::string String = "";
  std::string HEADER_END = "#LOOKUP_TABLE default";

  // READ HEADER OF THE FILE
  do {
     std::getline( File, String );
  } while ( String != HEADER_END );

  // read parameters
  std::getline( File, String );
  unsigned TempLength[ Dimensions ] = { 0 };
  getLengthFromString( TempLength, String );

  unsigned yLENGTH_MINIMUM = 2;
  if ( Length[ 1 ] < yLENGTH_MINIMUM ) {
    std::cout << "WARNING: the minimum yLength value must be " << yLENGTH_MINIMUM << std::endl;
    std::cout << "the corresponding value will be automatically reassigned to the minimum value\n"
              << "since the provided yLength is less than required"
              << std::endl;

    Length[ 1 ] = yLENGTH_MINIMUM;
  }
  Length[0] = TempLength[0];
  Length[2] = TempLength[2];

  // allocate the momory
  allocateFields( collideField,
  			  	  streamField,
  				  flagField,
  				  IdField,
  				  Length );



    const int DEFAULT_ID = -1;
    unsigned Current_Field_Cell = 0;
	// init Fields: collide and stream fields
	//Initialization of collideField
	for( unsigned z = 0 ; z < Length[ 2 ] + 2; ++z )  {
		for( unsigned y = 0 ; y < Length[ 1 ] + 2 ; ++y )  {
			for( unsigned x = 0 ; x < Length[ 0 ] + 2 ; ++x ) {

                ( *IdField )[ computeFlagIndex( x, y, z, Length ) ] = DEFAULT_ID;

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




  // read the file and fill in the flag field
  const char DELIMITER = ' ';
  int FLAG = FLUID;

  int LineCounter = 0;
  int  CharacterCounter = 0;

    while ( !File.eof() ) {

      getline (File,String);

      for ( std::string::iterator it = String.begin(); it < String.end(); ++it ) {

               if ( ( *it ) == '0' ) {
                   FLAG = FLUID;
                   ++CharacterCounter;
               }
               else if ( ( *it ) == '1' ) {
                   ++CharacterCounter;
                   FLAG = NO_SLIP;

               }
               else if ( ( *it ) == DELIMITER ) {
                   continue;
               }


               for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y  ) {
               (*flagField ) [ computeFlagIndex( CharacterCounter + 1,
                                                 y,
                                                 LineCounter + 1,
                                                 Length ) ] = FLAG;
               }
        }

        CharacterCounter = 0;
        ++LineCounter;
    }




  // initialised XY: INLET AND OUTLET
  for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
      for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
          ( *flagField )[ computeFlagIndex( x, y, 0, Length ) ] = INFLOW;
          ( *flagField )[ computeFlagIndex( x, y, Length[ 2 ] + 1, Length ) ] = OUTFLOW;
      }
  }

  // initialised ZX: INLET AND OUTLET
  for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
      for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
          ( *flagField )[ computeFlagIndex( x, 0, z, Length ) ] = FREE_SLIP;
          ( *flagField )[ computeFlagIndex( x, Length[ 1 ] + 1, z, Length ) ] = FREE_SLIP;
      }
  }


  // initialised ZY: INLET AND OUTLET
  for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
      for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
          ( *flagField )[ computeFlagIndex( 0, y, z, Length ) ] = NO_SLIP;
          ( *flagField )[ computeFlagIndex( Length[ 0 ] + 1, y, z, Length ) ] = NO_SLIP;
      }
  }
}
