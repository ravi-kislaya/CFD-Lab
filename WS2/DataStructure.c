#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include <list>

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
void StationaryWall::treatBoundary( double * Field ) {
    // TODO: write iplementation
	int Initial_Velocity_Component = getVelocityComponent();
	int Reflected_Vel_Component = 2 * ( 9 - Initial_Velocity_Component );
	Field[ getTargetIndex() + Reflected_Vel_Component ] = 
				Field[ getSelfIndex() + Initial_Velocity_Component ];
}


void MovingWall::treatBoundary( double * Field ) {
    // TODO: write iplementation
	double Density = 0.0;
	int Initial_Velocity_Component = getVelocityComponent();
	int Reflected_Vel_Component = 2 * ( 9 - Initial_Velocity_Component );
	computeDensity( Field + getTargetIndex(), &Density );
	Field[ getTargetIndex() + Reflected_Vel_Component ] = 
				Field[ getSelfIndex() + Initial_Velocity_Component ]
			+ ( 2.0 * LATTICEWEIGHTS[getVelocityComponent()] * Density 
				*getDotProduct() * InverseCS_Square );
	
}


//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
void Fluid::addObstacle( Obstacle* Obj ) {
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
