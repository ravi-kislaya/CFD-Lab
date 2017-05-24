#include "DataStructure.h"
#include "helper.h"
#include <list>

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
void StationaryWall::treatBoundary( double * Field ) {
    // TODO: write iplementation
}


void MovingWall::treatBoundary( double * Field ) {
    // TODO: write iplementation
}


//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
void Fluid::addObestacle( Obstacle* Obj ) {
    ObstacleList.push_back( Obj );
}


void Fluid::processBoundary( double * Field ) {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {
              (*Iterator)->treatBoundary( Field );
    }
}


void Fluid::deleteObstacles() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {

        delete ( *Iterator );
    }
}
