#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include <list>
#include <math.h>



//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
void StationaryWall::treatBoundary( double * Field ) {
    // TODO: write iplementation

	int Reflected_Vel_Component = abs( 18 - m_VelocityComponent );

	Field[ m_TargetIndex + Reflected_Vel_Component ]
                                = Field[ m_SelfIndex + m_VelocityComponent ];
}


void MovingWall::treatBoundary( double * Field ) {
    // TODO: write iplementation
	double Density = 0.0;
	int Reflected_Vel_Component = abs( 18 - m_VelocityComponent );

	computeDensity( Field + m_TargetIndex, &Density );

	Field[ m_TargetIndex + Reflected_Vel_Component ]
			          = Field[ m_SelfIndex + m_VelocityComponent ]
			          + ( 2.0 * LATTICEWEIGHTS[ m_VelocityComponent ] * Density
				           * m_DotProduct * InverseCS_Square );

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
