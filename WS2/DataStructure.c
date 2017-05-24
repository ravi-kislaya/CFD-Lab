#include "DataStructure.h"
#include <list>




void Fluid::addObestacle( Obstacle* Obj ) {
    ObstacleList.push_back( Obj );
}


void Fluid::processBoundary() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {
              (*Iterator)->treatBoundary();
    }
}


void Fluid::deleteObstacles() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {

        delete ( *Iterator );
    }
}
