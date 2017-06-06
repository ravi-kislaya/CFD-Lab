#include <list>
#include "LBDefinitions.h"

#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_SourceIndex( 0 ),
					 m_VelocityComponent( 0 ), m_DotProduct( 0.0 ) {}
        Obstacle( int SelfIndex,
                  int getSourceIndex,
                  int Component,
				  double DotProduct) : m_SelfIndex( SelfIndex ),
                                    m_SourceIndex( getSourceIndex ),
                                    m_VelocityComponent( Component ),
									m_DotProduct( DotProduct ) {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field ) = 0;


        int getSelfIndex() { return m_SelfIndex; }
        int getSourceIndex() { return m_SourceIndex; }
		int getVelocityComponent() { return m_VelocityComponent; }
		double getDotProduct() { return m_DotProduct; }

    protected:
        int m_SelfIndex;
        int m_SourceIndex;
        int m_VelocityComponent;
		double m_DotProduct;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex,
                        int getSourceIndex,
                        int Component,
						double DotProduct) : Obstacle( SelfIndex,
                                                    getSourceIndex,
                                                    Component,
													DotProduct) {}

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
};

class FreeSlip : public Obstacle {
    public:
        FreeSlip( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};

class Inflow : public Obstacle {
    public:
        Inflow( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct,
					double* Inlet ) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {
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
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};

class PressureIn : public Obstacle {
    public:

        PressureIn( int SelfIndex,
                    int getSourceIndex,
                    int Component,
					double DotProduct,
                    double DeltaDensity) : Obstacle( SelfIndex,
                                                getSourceIndex,
                                                Component,
												DotProduct),
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
				m_Index[i] = 0;
			}
		}

		Fluid( int ID, int x, int y, int z, int* Index ) :  m_ID( ID ),
                                                            m_XCoord( x ),
                                                            m_YCoord( y ),
                                                            m_ZCoord( z ) {
            // Assign all neighbours
			for( int i = 0; i < Vel_DOF; ++i ) {
				m_Index[ i ] = Index[ i ];
			}
		}

        int getID() { return m_ID; }
        int getXCoord() { return m_XCoord; }
        int getYCoord() { return m_YCoord; }
        int getZCoord() { return m_ZCoord; }
        int getIndex( int Index ) { return m_Index[ Index ]; }
        int getIdIndex( int Index ) { return (int)( m_Index[ Index ] / Vel_DOF ); }

	private:
        int m_ID;
        int m_XCoord;
        int m_YCoord;
        int m_ZCoord;
        int m_Index[ Vel_DOF ];


};

#endif
