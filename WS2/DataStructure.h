#include <list>


#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_TargetIndex( 0 ), 
					 m_VelocityComponent( 0 ), m_DotProduct( 0.0 ) {}
        Obstacle( int SelfIndex,
                  int getTargetIndex,
                  int Component,
				  double DotProduct) : m_SelfIndex( SelfIndex ),
                                    m_TargetIndex( getTargetIndex ),
                                    m_VelocityComponent( Component ),
									m_DotProduct( DotProduct ) {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field ) = 0;


        int getSelfIndex() { return m_SelfIndex; }
        int getTargetIndex() { return m_TargetIndex; }
		int getVelocityComponent() { return m_VelocityComponent; }
		double getDotProduct() { return m_DotProduct; }
    protected:
        int m_SelfIndex;
        int m_TargetIndex;
        int m_VelocityComponent;
		double m_DotProduct;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex,
                        int getTargetIndex,
                        int Component,
						double DotProduct) : Obstacle( SelfIndex,
                                                    getTargetIndex,
                                                    Component,
													DotProduct) {}

        virtual void treatBoundary( double * Field );
};


class MovingWall : public Obstacle {
    public:
        MovingWall( int SelfIndex,
                    int getTargetIndex,
                    int Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                getTargetIndex,
                                                Component,
												DotProduct) {}

        virtual void treatBoundary( double * Field );
};



//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
class Fluid {
    public:
        Fluid() : m_Coordinate( 0 ) {}
        Fluid( int Coordinate ) : m_Coordinate( Coordinate ) {}

        void addObstacle( Obstacle* Obj );
        void processBoundary( double * Field );
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }
        int getCoodinate() { return m_Coordinate; }

    private:
        int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};

#endif
