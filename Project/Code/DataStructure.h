#include <list>
#include "LBDefinitions.h"
#include "computeCellValues.h"

#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_SourceIndex( 0 ),
					 m_VelocityComponent( 0 ) {}
        Obstacle( int SelfIndex,
                  int getSourceIndex,
                  int Component ) : m_SelfIndex( SelfIndex ),
                                    m_SourceIndex( getSourceIndex ),
                                    m_VelocityComponent( Component ) {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field ) = 0;


        int getSelfIndex() { return m_SelfIndex; }
        int getSourceIndex() { return m_SourceIndex; }
		int getVelocityComponent() { return m_VelocityComponent; }

    protected:
        int m_SelfIndex;
        int m_SourceIndex;
        int m_VelocityComponent;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex,
                        int getSourceIndex,
                        int Component) : Obstacle( SelfIndex,
                                                   getSourceIndex,
                                                   Component ) {}

        virtual void treatBoundary( double * Field );
};


class MovingWall : public Obstacle {
    public:
        MovingWall( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
		double getDotProduct() { return m_DotProduct; }
	private:
		double m_DotProduct;
};

class FreeSlip : public Obstacle {
    public:
        FreeSlip( int SelfIndex,
                    int getSourceIndex,
                    int Component) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component ) {}

        virtual void treatBoundary( double * Field );
};

class Inflow : public Obstacle {
    public:
        Inflow( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double* Inlet ) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component ) {
						m_InletVelocity[0] = Inlet[0];
						m_InletVelocity[1] = Inlet[1];
						m_InletVelocity[2] = Inlet[2];
					}

        virtual void treatBoundary( double * Field );
	private:
		double m_InletVelocity[ Dimensions ];
};

class Outflow : public Obstacle {
    public:
        Outflow( int SelfIndex,
                    int getSourceIndex,
                    int Component) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component ) {}

        virtual void treatBoundary( double * Field );
};

class PressureIn : public Obstacle {
    public:

        PressureIn( int SelfIndex,
                    int getSourceIndex,
                    int Component,
                    double DeltaDensity) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component),
                                         m_DeltaDensity( DeltaDensity ) {}


        virtual void treatBoundary( double * Field );

    private:
        double m_DeltaDensity;
};



//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
class BoundaryFluid {
    public:
        BoundaryFluid() : m_Coordinate( 0 ) {}
        BoundaryFluid( int Coordinate ) : m_Coordinate( Coordinate ) {}

        void addObstacle( Obstacle* Obj );
        void processBoundary( double * Field );
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }
        int getCoodinate() { return m_Coordinate; }

    protected:
        int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};


class Fluid {
	public:
		Fluid(  ) {
			for( int i = 0; i < Vel_DOF; ++i ) {
				m_NeighbourIndex[i] = 0;
			}
		}

		Fluid( int ID, int x, int y, int z, int* Index ) :  m_ID( ID ),
                                                            m_XCoord( x ),
                                                            m_YCoord( y ),
                                                            m_ZCoord( z ) {
            // Assign all neighbours
			for( int i = 0; i < Vel_DOF; ++i ) {
				m_NeighbourIndex[ i ] = Index[ i ];
			}
		}

        int getID() { return m_ID; }
        int getXCoord() { return m_XCoord; }
        int getYCoord() { return m_YCoord; }
        int getZCoord() { return m_ZCoord; }
        int getIndex( int Index ) { return m_NeighbourIndex[ Index ]; }
        int getIdIndex( int Index ) { return (int)( m_NeighbourIndex[ Index ] / Vel_DOF ); }
		void doLocalStreaming( double* collideField, double* streamField );
		void doLocalCollision( double *collideField, double Inverse_Tau,
							   double *Density, double *Velocity, double* Feq );

	private:
        int m_ID;
        int m_XCoord;
        int m_YCoord;
        int m_ZCoord;
        int m_NeighbourIndex[ Vel_DOF ];

};

#endif
