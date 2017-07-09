#include <stdlib.h>
#include <list>
#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include <unordered_map>

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
 					std::vector<BoundaryEntry*> BoundaryConditions,
 					int *CpuID,
 					int RANK,
 					std::unordered_map<unsigned, unsigned>& LocalToGlobalIdTable,
 					std::vector<BoundaryBuffer>& CommunicationBuffers ) {


	int ID = 0;
	int isLatticeUnDrawable = 0;
	std::unordered_map<unsigned, unsigned>::const_iterator IdIterator;


	int Current_Local_Cell_Flag = 0;
	int Current_Local_Cell_Field = 0;

    int Neighbour_Global_Cell_Flag = 0;
	int Neighbour_Local_Cell_Flag = 0;
    int Neighbour_Local_Cell_Field = 0;

	double Dot_Product = 0.0;
	double WallVelocity[ 3 ] = { 0.0, 0.0, 0.0};
	double InletVelocity[ 3 ] = { 0.0, 0.0, 0.0};

	int BoundaryID = -1;
	int BoundaryType = -1;

	const int VTK_NEIGHBOR_SIZE = 6;
	int VtkNeighbors[ VTK_NEIGHBOR_SIZE ] = { 10, 12, 13, 16, 17, 18 };

	int NeighborCpuID = -1;


    for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell, ++ID ) {

		// Compute the current cell
		// IMPORTANT: the flag field is give in the glabal cell id whereas
		// collide and steam field are give in the local coordinates
		// IMPORTANT: all Fluid cells contain the local cell IDs
		Current_Local_Cell_Flag = ( *aFluidCell )->getIndex( SELF_INDEX );
		Current_Local_Cell_Field = Vel_DOF * Current_Local_Cell_Flag;

		// Allocate a new fluid cell
		BoundaryFluid* aBoundaryFluidCell = new BoundaryFluid( Current_Local_Cell_Field );

//.............................. SCAN PART: START ..............................
		for ( int i = 0; i < Vel_DOF; ++i ) {

//.............................. PREPARATION ..................................

			Neighbour_Local_Cell_Flag = ( *aFluidCell )->getIndex( i );
			IdIterator = LocalToGlobalIdTable.find( Neighbour_Local_Cell_Flag );
			Neighbour_Global_Cell_Flag = IdIterator->second;

			BoundaryID = flagField[ Neighbour_Global_Cell_Flag ];
			BoundaryType = BoundaryConditions[ BoundaryID ]->TYPE;

			NeighborCpuID = CpuID[ Neighbour_Global_Cell_Flag ];




//.............................. COMPARISON ..................................
			// fill all communication buffers with corresponding lattices
			if ( ( BoundaryType == FLUID ) && ( NeighborCpuID != RANK ) ) {
				CommunicationBuffers[ NeighborCpuID ].setTragetCpu( NeighborCpuID );
				CommunicationBuffers[ NeighborCpuID ].addBufferElement( Current_Local_Cell_Field + i );
			}


			// add neighbours cell ( which is wall or moving wall to the list )
			//No slip
			if ( BoundaryType == NO_SLIP ) {

				Neighbour_Local_Cell_Field = Vel_DOF * Neighbour_Local_Cell_Flag;

				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Wall = new StationaryWall( Neighbour_Local_Cell_Field,
                                                     Current_Local_Cell_Field,
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

				Neighbour_Local_Cell_Field = Vel_DOF * Neighbour_Local_Cell_Flag;


				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Wall = new MovingWall( Neighbour_Local_Cell_Field,
												 Current_Local_Cell_Field,
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

				Neighbour_Local_Cell_Field = Vel_DOF * Neighbour_Local_Cell_Flag;

				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Obstacle* Inlet = new Inflow( Neighbour_Local_Cell_Field,
											  Current_Local_Cell_Field,
											  18 - i,
											  InletVelocity );

				aBoundaryFluidCell->addObstacle( Inlet );

			}

			if ( BoundaryType == OUTFLOW ) {


				// IMPORTANT: Neighbour_Cell_Field means a wall lattice
				//			  Current_Cell_Field means a fluid lattice
				Neighbour_Local_Cell_Field = Vel_DOF * Neighbour_Local_Cell_Flag;
				Obstacle* Outlet = new Outflow( Neighbour_Local_Cell_Field,
											    Current_Local_Cell_Field,
											    18 - i );
				aBoundaryFluidCell->addObstacle( Outlet );

			}

		}
//.............................. SCAN PART: END ..............................


//.............................. VTK PART: START ...............................
		// Check out all neighbors in the positive octant
		// if all of them are fuild lattices then add the current lattice
		// to the represenation list
		isLatticeUnDrawable = 0;
		for ( int i = 0; i < VTK_NEIGHBOR_SIZE; ++i ) {

			// convert the local ID to the global one to iterate through
			// both flag field and partitioning field
			IdIterator = LocalToGlobalIdTable.find(  (*aFluidCell)->getIdIndex( VtkNeighbors[ i ] ) );
			BoundaryID = flagField[ IdIterator->second ];

			// find out the type of boundary condition
			// IMPORTANT: Fluid coresponds to the BC type that equals to zero
			isLatticeUnDrawable += BoundaryConditions[ BoundaryID ]->TYPE;

			if ( CpuID[ IdIterator->second ] != RANK ) {
				isLatticeUnDrawable += true;
			}
		}

		// process the diagonal element in the same manner as above
		IdIterator = LocalToGlobalIdTable.find(  (*aFluidCell)->getDiagonalLattice() );
		BoundaryID = flagField[ IdIterator->second ];
		isLatticeUnDrawable += BoundaryConditions[ BoundaryID ]->TYPE;

		if ( CpuID[ IdIterator->second ] != RANK ) {
			isLatticeUnDrawable += true;
		}


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
                    std::list<BoundaryFluid*>& BoundaryLayerList ) {


    // iterate through out all boundary layer cells
    for ( std::list<BoundaryFluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

    }
}
