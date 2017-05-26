#include "boundary.h"
#include "LBDefinitions.h"
#include <stdlib.h>

#include "DataStructure.h"
#include <list>
#include <iostream>
#include <stdio.h>
#include "helper.h"

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    int xlength,
                    double* wallVelocity ) {

    int Current_Cell_Flag = 0;
    int Neighbour_Cell_Flag = 0;
	int Current_Cell_Field = 0;
    int Neighbour_Cell_Field = 0;
	double Dot_Product = 0.0;

    for( int z = 1 ; z <= xlength; ++z )  {
        for( int y = 1 ; y <= xlength; ++y )  {
            for( int x = 1 ; x <= xlength; ++x ) {

                // Compute the current cell
                Current_Cell_Flag = computeFlagIndex( x, y, z, xlength );
				Current_Cell_Field = computeFieldIndex( x, y, z, xlength );

                // Allocate a new fluid cell
                Fluid* aFluidCell = new Fluid( Current_Cell_Flag );

                // scan neighbours
                for ( int i = 0; i < 19; ++i ) {


                    Neighbour_Cell_Flag = computeFlagIndex(  x + LATTICEVELOCITIES[ i ][ 0 ],
                                                        y + LATTICEVELOCITIES[ i ][ 1 ],
                                                        z + LATTICEVELOCITIES[ i ][ 2 ],
                                                        xlength );


                    // add neighbours cell ( which is wall or moving wall to the list )
                    if ( flagField[ Neighbour_Cell_Flag ] == WALL ) {
						
						Dot_Product = 0.0;
						Neighbour_Cell_Field = computeFieldIndex(  x + LATTICEVELOCITIES[ i ][ 0 ],
                                                        y + LATTICEVELOCITIES[ i ][ 1 ],
                                                        z + LATTICEVELOCITIES[ i ][ 2 ],
                                                        xlength );
                        Obstacle* Wall = new StationaryWall( Neighbour_Cell_Field, //Here,Neighbour means wall
                                                             Current_Cell_Field,   //Here,Current means fluid
                                                             i,
                                                             Dot_Product );
                        aFluidCell->addObstacle( Wall );

                        }


                    if ( flagField[ Neighbour_Cell_Flag ] == MOVING_WALL ) {
						
						Dot_Product = ( wallVelocity[0] * LATTICEVELOCITIES[i][0] )
									+ ( wallVelocity[1] * LATTICEVELOCITIES[i][1] )
									+ ( wallVelocity[2] * LATTICEVELOCITIES[i][2] );
									
						
						
						Neighbour_Cell_Field = computeFieldIndex(  x + LATTICEVELOCITIES[ i ][ 0 ],
                                                        y + LATTICEVELOCITIES[ i ][ 1 ],
                                                        z + LATTICEVELOCITIES[ i ][ 2 ],
                                                        xlength );

                        Obstacle* Wall = new MovingWall( Neighbour_Cell_Field,//Here,Neighbour means wall
                                                         Current_Cell_Field,  //Here,Current means fluid
                                                         i,
                                                         Dot_Product );
                        aFluidCell->addObstacle( Wall );
                    }

                }

                // Delete a fluid cell if there was no obstacle cells
                if ( aFluidCell->isEmpty() == true ) {
                    delete aFluidCell;
                }
                else {
                    ObstacleList.push_back( aFluidCell );
                }

            }
        }
    }

}



void treatBoundary( double *collideField,
                    std::list<Fluid*>& BoundaryLayerList,
                    const double * const wallVelocity,
                    int xlength ) {


    // iterate through out all boundary layer cells
    for ( std::list<Fluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

    }
}
