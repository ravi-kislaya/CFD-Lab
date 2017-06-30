#include <stdlib.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <typeinfo>

#include "helper.h"
#include "boundary.h"
#include "LBDefinitions.h"
#include "DataStructure.h"

//std::list<Fluid*>FluidDomain has to be initialised in initLB.cpp
void scanBoundary(  std::list<BoundaryFluid*>& ObstacleList,
					std::vector<Fluid*>& FluidDomain,
					std::vector<Fluid*>& VTKrepresentation,
                    int* flagField,
					int *VtkID,
 					std::vector<BoundaryEntry*> BoundaryConditions ) {


	int ID = 0;
	int isLatticeUnDrawable = 0;

    int Current_Cell_Flag = 0;
	int Current_Cell_Field = 0;

    int Neighbour_Cell_Flag = 0;
    int Neighbour_Cell_Field = 0;

	int Neighbour[ Vel_DOF ] = { 0 };
	double Dot_Product = 0.0;
	int FreeSlipVelocity[ 6 ] = { 2, 6, 8, 10, 12, 16 };


	double WallVelocity[ 3 ] = { 0.0, 0.0, 0.0};
	double InletVelocity[ 3 ] = { 0.0, 0.0, 0.0};
	int BoundaryID = -1;
	int BoundaryType = -1;

	const int VTK_NEIGHBOR_SIZE = 6;
	int VtkNeighbors[ VTK_NEIGHBOR_SIZE ] = { 10, 12, 13, 16, 17, 18 };

    for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell, ++ID ) {

		// Compute the current cell
        Current_Cell_Flag = ( *aFluidCell )->getIndex( SELF_INDEX );
		Current_Cell_Field = Vel_DOF * Current_Cell_Flag;

		// Allocate a new fluid cell
		BoundaryFluid* aBoundaryFluidCell = new BoundaryFluid( Current_Cell_Flag );

		// scan neighbours
		for ( int i = 0; i < Vel_DOF; ++i ) {


			Neighbour_Cell_Flag = ( *aFluidCell )->getIndex( i );

			BoundaryID = flagField[ Neighbour_Cell_Flag ];
			BoundaryType = BoundaryConditions[ BoundaryID ]->TYPE;


			// add neighbours cell ( which is wall or moving wall to the list )

			//No slip
			if ( BoundaryType == NO_SLIP ) {

				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Wall = new StationaryWall( Neighbour_Cell_Field,
                                                     Current_Cell_Field,
                                                     i );

				aBoundaryFluidCell->addObstacle( Wall );

            }


			//Moving Wall
			if ( BoundaryType == MOVING_WALL ) {

				// Explicitly assign the velocity components to a temp variables
				// to make the dot product formula look clear
				WallVelocity[ 0 ] = BoundaryConditions[ BoundaryID ]->Data[ 0 ];
				WallVelocity[ 1 ] = BoundaryConditions[ BoundaryID ]->Data[ 1 ];
				WallVelocity[ 2 ] = BoundaryConditions[ BoundaryID ]->Data[ 2 ];


				Dot_Product
					= ( WallVelocity[ 0 ] * LATTICEVELOCITIES[ 18 - i ][ 0 ] )
					+ ( WallVelocity[ 1 ] * LATTICEVELOCITIES[ 18 - i ][ 1 ] )
					+ ( WallVelocity[ 2 ] * LATTICEVELOCITIES[ 18 - i ][ 2 ] );

				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;


				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Wall = new MovingWall( Neighbour_Cell_Field,
												 Current_Cell_Field,
												 i,
												 Dot_Product );

				aBoundaryFluidCell->addObstacle( Wall );
			}


			//Inflow
			if ( BoundaryType == INFLOW ) {

				// Explicitly assign the velocity components to a temp variables
				// to make the definition of the INLET boundary condition look clear
				InletVelocity[ 0 ] = BoundaryConditions[ BoundaryID ]->Data[ 0 ];
				InletVelocity[ 1 ] = BoundaryConditions[ BoundaryID ]->Data[ 1 ];
				InletVelocity[ 2 ] = BoundaryConditions[ BoundaryID ]->Data[ 2 ];

				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;

				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Wall = new Inflow( Neighbour_Cell_Field,
											 Current_Cell_Field,
											 18 - i,
											 InletVelocity );

				aBoundaryFluidCell->addObstacle( Wall );

			}

/*
			//Pressure In
			if ( BoundaryType == PRESSURE_IN ) {

				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
				Obstacle* Wall = new PressureIn( Neighbour_Cell_Field,//Here,Neighbour means wall
												 Current_Cell_Field,  //Here,Current means fluid
												 18 - i,
												 DeltaDensity );

				//TODO:some global variable from readParameters
				aBoundaryFluidCell->addObstacle( Wall );


			}
*/

			//Outflow
			if ( BoundaryType == OUTFLOW ) {


				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
				Obstacle* Wall = new Outflow( Neighbour_Cell_Field,
											  Current_Cell_Field,
											  18 - i );
				aBoundaryFluidCell->addObstacle( Wall );

			}

		}

/*
		//separate loop for Free Slip
		for( int i = 0; i < 6; ++i ) {

			Neighbour_Cell_Flag = ( *aFluidCell )->getIndex( FreeSlipVelocity[ i ] );

			if( flagField[ Neighbour_Cell_Flag ] == FREE_SLIP ) {

				Neighbour_Cell_Field = Vel_DOF * Neighbour_Cell_Flag;
				Obstacle* Wall = new FreeSlip( Neighbour_Cell_Field,//Here,Neighbour means wall
											   Current_Cell_Field,  //Here,Current means fluid
											   FreeSlipVelocity[i] );
				aBoundaryFluidCell->addObstacle( Wall );

			}

		}
*/

//.............................. VTK PART: START ...............................
		// Check out all neighbors in the positive octant
		// if all of them are fuild lattices then add the current lattice
		// to the represenation list
		isLatticeUnDrawable = 0;
		for ( int i = 0; i < VTK_NEIGHBOR_SIZE; ++i ) {
			BoundaryID = flagField[ (*aFluidCell)->getIdIndex( VtkNeighbors[ i ] ) ];
			isLatticeUnDrawable += BoundaryConditions[ BoundaryID ]->TYPE;
		}


		BoundaryID = flagField[ (*aFluidCell)->getDiagonalLattice() ];
		isLatticeUnDrawable += BoundaryConditions[ BoundaryID ]->TYPE;


		if ( isLatticeUnDrawable == false  ) {
			VTKrepresentation.push_back( (*aFluidCell) );
		}

		VtkID[ (*aFluidCell)->getID() ] = ID;

//................................... END .....................................


		// Delete a fluid cell if there was no obstacle cells
		if ( aBoundaryFluidCell->isEmpty() == true ) {
			delete aBoundaryFluidCell;
		}
		else {
			ObstacleList.push_back( aBoundaryFluidCell );
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
