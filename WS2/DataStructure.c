#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include <list>



//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
inline void StationaryWall::treatBoundary( double * Field ) {


	int Reflected_Vel_Component = 18 - m_VelocityComponent;



	Field[ m_SelfIndex + Reflected_Vel_Component ]
                                = Field[ m_SourceIndex + m_VelocityComponent ];
}

inline void MovingWall::treatBoundary( double * Field ) {

	double Density = 0.0;
	int Reflected_Vel_Component = 18 - m_VelocityComponent;


	computeDensity( Field + m_SourceIndex, &Density );

	Field[ m_SelfIndex + Reflected_Vel_Component ]
			          = Field[ m_SourceIndex + m_VelocityComponent ]
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
              ( *Iterator)->treatBoundary( Field );
    }
}


void Fluid::deleteObstacles() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {

        delete ( *Iterator );
    }
}
