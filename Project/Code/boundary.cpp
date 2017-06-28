#include <stdlib.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <typeinfo>

#include "helper.h"
#include "boundary.h"
#include "LBDefinitions.h"
#include "DataStructure.h"


void scanBoundary( 	std::vector<Fluid*>& FluidDomain,
					std::vector<Fluid*>& VTKrepresentation,
					std::list<BoundaryFluid*>& ObstacleList,
					int* flagField,
					int* VtkID,
					std::vector<BoundaryEntry*> &BoundaryConditions ) {


	int ID = 0;
	int isLatticeUnDrawable = 0;
	for( unsigned i = 0; i < FluidDomain.size(); ++i, ++ID ) {

/*
		for( int Component = 0; i < Vel_DOF; ++Component ) {
			FluidDomain[ i ]->getIdIndex( Component );
		}
*/


		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 10 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 12 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 13 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 16 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 17 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getIdIndex( 18 ) ];
		isLatticeUnDrawable += flagField[ FluidDomain[ i ]->getDiagonalLattice() ];



		if ( isLatticeUnDrawable == false  ) {
			VTKrepresentation.push_back( FluidDomain[ i ] );
		}

		VtkID[ FluidDomain[ i ]->getID() ] = ID;
		isLatticeUnDrawable = 0;
	}


/*
	int ID = 0;
    int Current_Cell_Flag = 0;
    int Neighbour_Cell_Flag = 0;
	int Current_Cell_Field = 0;
    int Neighbour_Cell_Field = 0;
	int Neighbour[ Vel_DOF ] = { 0 };
	double Dot_Product = 0.0;
	int FreeSlipVelocity[6] = { 2, 6, 8, 10, 12, 16 };


    for( unsigned z = 0 ; z <= Length[ 2 ] + 1; ++z )  {
        for( unsigned y = 0 ; y <= Length[ 1 ] + 1; ++y )  {
            for( unsigned x = 0 ; x <= Length[ 0 ] + 1; ++x ) {

				// Compute the current cell
                Current_Cell_Flag = computeFlagIndex( x, y, z, Length );
				Current_Cell_Field = Vel_DOF * Current_Cell_Flag;

				if( flagField[Current_Cell_Flag] == FLUID ) {

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

						//Inflow
						if ( flagField[ Neighbour_Cell_Flag ] == INFLOW ) {
							Dot_Product = 0.0;
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
							Obstacle* Wall = new Inflow( Neighbour_Cell_Field,//Here,Neighbour means wall
														 Current_Cell_Field,  //Here,Current means fluid
														 18 - i,
														 Dot_Product,
														 InletVel );


							aBoundaryFluidCell->addObstacle( Wall );


						}

						//Pressure In
						if ( flagField[ Neighbour_Cell_Flag ] == PRESSURE_IN ) {
							Dot_Product = 0.0;
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
							Obstacle* Wall = new PressureIn( Neighbour_Cell_Field,//Here,Neighbour means wall
															 Current_Cell_Field,  //Here,Current means fluid
															 18 - i,
															 Dot_Product,
														     DeltaDensity );

							//TODO:some global variable from readParameters
							aBoundaryFluidCell->addObstacle( Wall );


						}

						//Outflow
						if ( flagField[ Neighbour_Cell_Flag ] == OUTFLOW ) {
							Dot_Product = 0.0;
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
							Obstacle* Wall = new Outflow( Neighbour_Cell_Field,//Here,Neighbour means wall
														  Current_Cell_Field,  //Here,Current means fluid
														  18 - i,
														  Dot_Product );
							aBoundaryFluidCell->addObstacle( Wall );

						}

					}

					//separate loop for Free Slip
					for( int i = 0; i < 6; ++i ) {
						Neighbour_Cell_Flag = computeFlagIndex( x + LATTICEVELOCITIES[ FreeSlipVelocity[i] ][ 0 ],
																y + LATTICEVELOCITIES[ FreeSlipVelocity[i] ][ 1 ],
																z + LATTICEVELOCITIES[ FreeSlipVelocity[i] ][ 2 ],
																Length );

						if( flagField[ Neighbour_Cell_Flag ] == FREE_SLIP ) {
							Dot_Product = 0.0;
							Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
							Obstacle* Wall = new FreeSlip( Neighbour_Cell_Field,//Here,Neighbour means wall
														   Current_Cell_Field,  //Here,Current means fluid
														   FreeSlipVelocity[i],
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
*/
}
