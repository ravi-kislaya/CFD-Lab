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
                        Obstacle* Wall = new StationaryWall( Neighbour_Cell, Current_Cell, i );
                        aFluidCell->addObestacle( Wall );

                        }


                    if ( flagField[ Neighbour_Cell ] == MOVING_WALL ) {
                        Obstacle* Wall = new MovingWall( Neighbour_Cell, Current_Cell, i );
                        aFluidCell->addObestacle( Wall );
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


    // iterate through out all bounsry layer cells
    for ( std::list<Fluid*>::iterator FluidCell = BoundaryLayerList.begin();
          FluidCell != BoundaryLayerList.end();
          ++FluidCell ) {

              (*FluidCell)->processBoundary( collideField );

    }
}
