#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include "LBDefinitions.h"
#include <list>



//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------

/********************StationaryWall************************************/
inline void StationaryWall::treatBoundary( double * Field ) {


	int Reflected_Vel_Component = 18 - m_VelocityComponent;



	Field[ m_SelfIndex + Reflected_Vel_Component ]
                                = Field[ m_SourceIndex + m_VelocityComponent ];
}

/********************MovingWall************************************/
inline void MovingWall::treatBoundary( double * Field ) {

	double Density = 0.0;
	int Reflected_Vel_Component = 18 - m_VelocityComponent;


	computeDensity( Field + m_SourceIndex, &Density );

	Field[ m_SelfIndex + Reflected_Vel_Component ]
			          = Field[ m_SourceIndex + m_VelocityComponent ]
					  + ( 2.0 * LATTICEWEIGHTS[ m_VelocityComponent ] * Density
				          * m_DotProduct * InverseCS_Square );


}

/********************FreeSlip************************************/
inline void FreeSlip::treatBoundary( double * Field ) {
	
	if(m_VelocityComponent == 2) {
		unsigned int Reflected_Vel_Component[5] = { 16, 18, 14, 17, 15 };
		unsigned int Source_Vel_Component[5]    = { 2, 4, 0, 3, 1 };
	}	
	else if(m_VelocityComponent == 6) {
		unsigned int Reflected_Vel_Component[5] = { 12, 18, 4, 13, 11 };
		unsigned int Source_Vel_Component[5]    = { 6, 14, 0, 7, 5 };
	}
	else if(m_VelocityComponent == 8) {
		unsigned int Reflected_Vel_Component[5] = { 10, 13, 7, 17, 3 };
		unsigned int Source_Vel_Component[5]    = { 8, 11, 5, 15, 1 };
	}
	else if(m_VelocityComponent == 10) {
		unsigned int Reflected_Vel_Component[5] = { 8, 11, 5, 15, 1 };
		unsigned int Source_Vel_Component[5]    = { 10, 13, 7, 17, 3 };
	}
	else if(m_VelocityComponent == 12) {
		unsigned int Reflected_Vel_Component[5] = { 6, 14, 0, 7, 5 };
		unsigned int Source_Vel_Component[5]    = { 12, 18, 4, 13, 11 };
	}
	else {
		unsigned int Reflected_Vel_Component[5] = { 2, 4, 0, 3, 1 };
		unsigned int Source_Vel_Component[5]    = { 16, 18, 14, 17, 15 };
	}

	for(int i=0; i < 5; ++i) {
		Field[ m_SelfIndex + Reflected_Vel_Component[i] ]
						= Field[ m_SourceIndex + Source_Vel_Component[i] ];
	}

}

/********************Inflow************************************/
inline void Inflow::treatBoundary( double * Field ) {
	
	computeSingleFeq( &Density_Reference, InletVelocity, Field + m_SelfIndex, m_VelocityComponent );
	
	
}

/********************PressureIn************************************/
inline void PressureIn::treatBoundary( double * Field ) {
	
	double Density = Density_Reference + DeltaDensity;
	
	computeVelocity( Field+m_SourceIndex, &Density, VelocityNeighbourFluid );
	computeSingleFeq( &Density, VelocityNeighbourFluid, &FeqI, m_VelocityComponent );
	computeSingleFeq( &Density, VelocityNeighbourFluid, &FeqInvI, 18 - m_VelocityComponent );
	
	Field[m_SelfIndex + m_VelocityComponent] = FeqI + FeqInvI 
											 - Field[m_SourceIndex + 18 - m_VelocityComponent];
	
}

/********************Outflow************************************/
inline void Outflow::treatBoundary( double * Field ) {
	
	double Density = 0.0;
	double VelocityNeighbourFluid[ Dimensions ] = {0.0};
	double FeqI = 0.0;
	double FeqInvI = 0.0;
	
	computeDensity( Field + m_SourceIndex, &Density );
	computeVelocity( Field + m_SourceIndex, &Density, VelocityNeighbourFluid );
	computeSingleFeq( &Density_Reference, VelocityNeighbourFluid, &FeqI, m_VelocityComponent );
	computeSingleFeq( &Density_Reference, VelocityNeighbourFluid, &FeqInvI, 18 - m_VelocityComponent );
	
	Field[m_SelfIndex + m_VelocityComponent] = FeqI + FeqInvI 
											 - Field[m_SourceIndex + 18 - m_VelocityComponent];
	
}


//------------------------------------------------------------------------------
//                            Boundary Fluid cell
//------------------------------------------------------------------------------
void BoundaryFluid::addObstacle( Obstacle* Obj ) {
    ObstacleList.push_back( Obj );
}


void BoundaryFluid::processBoundary( double * Field ) {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {
              ( *Iterator)->treatBoundary( Field );
    }
}


void BoundaryFluid::deleteObstacles() {
    for ( std::list<Obstacle*>::iterator Iterator = ObstacleList.begin();
          Iterator != ObstacleList.end();
          ++Iterator ) {

        delete ( *Iterator );
    }
}
