#include "boundary.h"
#include "LBDefinitions.h"
#include <stdlib.h>

#include "DataStructure.h"
#include <list>

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    int xlength,
                    double* wallVelocity ) {

    int Square_xlength = xlength * xlength;
    int Current_Cell = 0;
    int Neighbour_Cell = 0;

    for( int z = 1 ; z <= xlength; ++z )  {
        for( int y = 1 ; y <= xlength; ++y )  {
            for( int x = 1 ; x <= xlength; ++x ) {
                // Allocate a new fluid cell
                Fluid* aFluidCell = new Fluid;

                // Compute the current cell
                Current_Cell = ( z * Square_xlength ) + ( y * xlength ) + x;

                // scan neighbours
                for ( int i = 0; i < 19; ++i ) {
                    Neighbour_Cell = ( ( z + LATTICEVELOCITIES[ i ][ 2 ] ) * Square_xlength )
                                   + ( ( y + LATTICEVELOCITIES[ i ][ 1 ]  ) * xlength )
                                   + ( x  + LATTICEVELOCITIES[ i ][ 0 ] );

                    // add neighbours cell ( which is wall or moving wall to the list )
                    if ( flagField[ Neighbour_Cell ] == WALL ) {
                        Obstacle* Wall = new StationaryWall( Neighbour_Cell, Current_Cell );
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
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength ){

    int Square_xlength = xlength * xlength;
    int Current_Cell = 0;



    for( int Z_Coordinate = 1 ; Z_Coordinate < xlength ; ++Z_Coordinate )  {
		for( int Y_Coordinate = 1 ; Y_Coordinate < xlength ; ++Y_Coordinate )  {
			for( int X_Coordinate = 1 ; X_Coordinate < xlength ; ++X_Coordinate ) {


				Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

        for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
          collideField [Current_Cell + Vel_Component] = LATTICEWEIGHTS[Vel_Component] ;
        }

			}
		}
	}

}
