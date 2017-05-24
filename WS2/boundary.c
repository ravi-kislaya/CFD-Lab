#include "boundary.h"
#include "LBDefinitions.h"
#include <stdlib.h>

#include "DataStructure.h"
#include <list>
#include <iostream>
#include "helper.h"

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    int xlength,
                    double* wallVelocity ) {

    int Current_Cell = 0;
    int Neighbour_Cell = 0;
	double Dot_Product = 0.0;

    for( int z = 1 ; z <= xlength; ++z )  {
        for( int y = 1 ; y <= xlength; ++y )  {
            for( int x = 1 ; x <= xlength; ++x ) {

                // Compute the current cell
                Current_Cell = computeFlagIndex( x, y, z, xlength );

                // Allocate a new fluid cell
                Fluid* aFluidCell = new Fluid( Current_Cell );

                // scan neighbours
                for ( int i = 0; i < 19; ++i ) {


                    Neighbour_Cell = computeFlagIndex(  x + LATTICEVELOCITIES[ i ][ 0 ],
                                                        y + LATTICEVELOCITIES[ i ][ 1 ],
                                                        z + LATTICEVELOCITIES[ i ][ 2 ],
                                                        xlength );


                    // add neighbours cell ( which is wall or moving wall to the list )
                    if ( flagField[ Neighbour_Cell ] == WALL ) {
						Dot_Product = 0.0;
                        Obstacle* Wall = new StationaryWall( Neighbour_Cell, Current_Cell, i, Dot_Product );
                        aFluidCell->addObstacle( Wall );

                        }


                    if ( flagField[ Neighbour_Cell ] == MOVING_WALL ) {
						Dot_Product = ( wallVelocity[0] * LATTICEVELOCITIES[i][0] )
									+ ( wallVelocity[1] * LATTICEVELOCITIES[i][1] )
									+ ( wallVelocity[2] * LATTICEVELOCITIES[i][2] ) ;
                        Obstacle* Wall = new MovingWall( Neighbour_Cell, Current_Cell, i, Dot_Product );
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
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength ){


    // iterate through out all boundary layer cells
    for ( std::list<Fluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

    }
}
