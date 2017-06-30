#include <stdlib.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <typeinfo>

#include "helper.h"
#include "boundary.h"
#include "LBDefinitions.h"
#include "DataStructure.h"


void scanBoundary(  std::list<BoundaryFluid*>& ObstacleList,
					std::list<Fluid*>& FluidDomain,
					BoundaryBuffer* BoundaryBufferArray,
					std::list<Fluid*>& VTKrepresentation,
                    int* flagField,
					int *IdField,
                    unsigned* Length,
                    double* wallVelocity ) {


	int ID = 0;
    int Current_Cell_Flag = 0;
    int Neighbour_Cell_Flag = 0;
	int Current_Cell_Field = 0;
    int Neighbour_Cell_Field = 0;
	int Neighbour[ Vel_DOF ] = { 0 };
	double Dot_Product = 0.0;


    for( unsigned z = 0 ; z <= Length[ 2 ] + 1; ++z )  {
        for( unsigned y = 0 ; y <= Length[ 1 ] + 1; ++y )  {
            for( unsigned x = 0 ; x <= Length[ 0 ] + 1; ++x ) {

				// Compute the current cell
                Current_Cell_Flag = computeFlagIndex( x, y, z, Length );
				Current_Cell_Field = Vel_DOF * Current_Cell_Flag;

				if( ( flagField[Current_Cell_Flag] == FLUID ) ) {

					// Allocate a new fluid cell
					BoundaryFluid* aBoundaryFluidCell = new BoundaryFluid( Current_Cell_Flag );

					// scan neighbours
					for ( int i = 0; i < Vel_DOF; ++i ) {


						Neighbour_Cell_Flag = computeFlagIndex( x + LATTICEVELOCITIES[ i ][ 0 ],
																y + LATTICEVELOCITIES[ i ][ 1 ],
																z + LATTICEVELOCITIES[ i ][ 2 ],
																Length );

						Neighbour[ i ] = Vel_DOF * Neighbour_Cell_Flag;

						// add neighbours cell ( which is wall or moving wall to the list )

						//No slip
						if ( flagField[ Neighbour_Cell_Flag ] == NO_SLIP ) {

							Dot_Product = 0.0;
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

							Obstacle* Wall = new StationaryWall( Neighbour_Cell_Field, //Here,Neighbour means wall
                                                             Current_Cell_Field,   //Here,Current means fluid
                                                             i,
                                                             Dot_Product );

							aBoundaryFluidCell->addObstacle( Wall );

                        }

						//Moving Wall
						if ( flagField[ Neighbour_Cell_Flag ] == MOVING_WALL ) {

							Dot_Product = ( wallVelocity[0] * LATTICEVELOCITIES[ 18 - i ][0] )
										+ ( wallVelocity[1] * LATTICEVELOCITIES[ 18 - i ][1] )
										+ ( wallVelocity[2] * LATTICEVELOCITIES[ 18 - i ][2] );
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

							Obstacle* Wall = new MovingWall( Neighbour_Cell_Field,//Here,Neighbour means wall
															Current_Cell_Field,  //Here,Current means fluid
															i,
															Dot_Product );
							aBoundaryFluidCell->addObstacle( Wall );
						}
					}





					//Create and push the Fluid in the Fluid Domain List
					Fluid* aFluidCell = new Fluid( ID, x, y, z, Neighbour );
					FluidDomain.push_back( aFluidCell );

					// Update IdField according the field flag
					IdField[ Current_Cell_Flag ] = ID;

					// Update ID imediatly after pushing back a new fluid
					// element into the list
					++ID;


					// add a fluid cell to the represenation list
					//
					//	NestZ
					//	|
					//	|  NextY
					//	| /
					//	|/
					//	O----------- NextX
					//  CURRENT CELL
					//
					// IMPORTANT: be careful deleting FluidDomain and VTKrepresentation
					// lists since they contain common pointer

					// TODO: rename variables in such way that it should be readable

					int Index = 0;
					int isItDrawable = true;
					for ( int dz = 0; dz < 2; ++dz ) {
						for ( int dy = 0; dy < 2; ++dy ) {
							for ( int dx = 0; dx < 2; ++dx ) {

								Index = computeFlagIndex( x + dx,
														  y + dy,
														  z + dz,
														  Length );

								isItDrawable *= flagField[ Index ] == FLUID;
							}
						}
					}


					if ( isItDrawable == true ) {
						VTKrepresentation.push_back( aFluidCell );
					}



					// Delete a fluid cell if there was no obstacle cells
					if ( aBoundaryFluidCell->isEmpty() == true ) {
						delete aBoundaryFluidCell;
					}
					else {
						ObstacleList.push_back( aBoundaryFluidCell );
					}
				}

            }
        }
    }

    // init all neighbours
  	// Boundary Buffer Scheme:
  	// index 0: +x direction
  	// index 1: -x direction
  	// index 2: +y direction
  	// index 3: -y directionProc
  	// index 4: +z direction
  	// index 5: -z direction

	//communication on YZ direction: +x and -x
	for ( unsigned z = 1; z < Length[ 2 ] + 1; ++z ) {
		for ( unsigned y = 1; y < Length[ 1 ] + 1; ++y ) {
			Current_Cell_Field = computeFieldIndex( Length[0], y, z, Length );
			
            BoundaryBufferArray[ 0 ].addBufferElement( Current_Cell_Field + 10 );
            BoundaryBufferArray[ 0 ].addBufferElement( Current_Cell_Field  + 13 );
            BoundaryBufferArray[ 0 ].addBufferElement( Current_Cell_Field + 7 );
            BoundaryBufferArray[ 0 ].addBufferElement( Current_Cell_Field + 17 );
            BoundaryBufferArray[ 0 ].addBufferElement( Current_Cell_Field + 3 );

		}
	}

	for ( unsigned z = 1; z < Length[ 2 ] + 1; ++z ) {
		for ( unsigned y = 1; y < Length[ 1 ] + 1; ++y ) {
			Current_Cell_Field = computeFieldIndex( 1, y, z, Length );
            
            BoundaryBufferArray[ 1 ].addBufferElement( Current_Cell_Field + 8 );
            BoundaryBufferArray[ 1 ].addBufferElement( Current_Cell_Field  + 11 );
            BoundaryBufferArray[ 1 ].addBufferElement( Current_Cell_Field + 5 );
            BoundaryBufferArray[ 1 ].addBufferElement( Current_Cell_Field + 15 );
            BoundaryBufferArray[ 1 ].addBufferElement( Current_Cell_Field + 1 );



		}
	}


	//communication on XZ direction: +y and -y
	for ( unsigned z = 1; z < Length[ 2 ] + 1; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			Current_Cell_Field = computeFieldIndex( x, Length[1], z, Length );


            BoundaryBufferArray[ 2 ].addBufferElement( Current_Cell_Field + 12 );
            BoundaryBufferArray[ 2 ].addBufferElement( Current_Cell_Field  + 4 );
            BoundaryBufferArray[ 2 ].addBufferElement( Current_Cell_Field + 18 );
            BoundaryBufferArray[ 2 ].addBufferElement( Current_Cell_Field + 13 );
            BoundaryBufferArray[ 2 ].addBufferElement( Current_Cell_Field + 11 );


		}
	}

	for ( unsigned z = 1; z < Length[ 2 ] + 1; ++z ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			Current_Cell_Field = computeFieldIndex( x, 1, z, Length );

            BoundaryBufferArray[ 3 ].addBufferElement( Current_Cell_Field + 6 );
            BoundaryBufferArray[ 3 ].addBufferElement( Current_Cell_Field  + 0 );
            BoundaryBufferArray[ 3 ].addBufferElement( Current_Cell_Field + 14 );
            BoundaryBufferArray[ 3 ].addBufferElement( Current_Cell_Field + 7 );
            BoundaryBufferArray[ 3 ].addBufferElement( Current_Cell_Field + 5 );


        }
	}

	//communication on XY direction: +z and -z
	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			Current_Cell_Field = computeFieldIndex( x, y, Length[2], Length );

            BoundaryBufferArray[ 4 ].addBufferElement( Current_Cell_Field + 16 );
            BoundaryBufferArray[ 4 ].addBufferElement( Current_Cell_Field  + 18 );
            BoundaryBufferArray[ 4 ].addBufferElement( Current_Cell_Field + 14 );
            BoundaryBufferArray[ 4 ].addBufferElement( Current_Cell_Field + 17 );
            BoundaryBufferArray[ 4 ].addBufferElement( Current_Cell_Field + 15 );

		}
	}

	for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
		for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {
			Current_Cell_Field = computeFieldIndex( x, y, 1, Length );

            BoundaryBufferArray[ 5 ].addBufferElement( Current_Cell_Field + 2 );
            BoundaryBufferArray[ 5 ].addBufferElement( Current_Cell_Field  + 4 );
            BoundaryBufferArray[ 5 ].addBufferElement( Current_Cell_Field + 0 );
            BoundaryBufferArray[ 5 ].addBufferElement( Current_Cell_Field + 3 );
            BoundaryBufferArray[ 5 ].addBufferElement( Current_Cell_Field + 1 );

		}
	}

}



void treatBoundary( double *collideField,
                    std::list<BoundaryFluid*>& BoundaryLayerList,
                    const double * const wallVelocity ) {


    // iterate through out all boundary layer cells
    for ( std::list<BoundaryFluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

        }  
}

