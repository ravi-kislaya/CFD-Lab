#include <list>


#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_SourceIndex( 0 ),
					 m_VelocityComponent( 0 ), m_DotProduct( 0.0 ) {}
        Obstacle( unsigned long int SelfIndex,
                  unsigned long int getSourceIndex,
                  unsigned Component,
				  double DotProduct) : m_SelfIndex( SelfIndex ),
                                    m_SourceIndex( getSourceIndex ),
                                    m_VelocityComponent( Component ),
									m_DotProduct( DotProduct ) {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field ) = 0;


        unsigned long int getSelfIndex() { return m_SelfIndex; }
        unsigned long int getSourceIndex() { return m_SourceIndex; }
		unsigned getVelocityComponent() { return m_VelocityComponent; }
		double getDotProduct() { return m_DotProduct; }

    protected:
        unsigned long int m_SelfIndex;
        unsigned long int m_SourceIndex;
        unsigned m_VelocityComponent;
		double m_DotProduct;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( unsigned long int SelfIndex,
                        unsigned long int getSourceIndex,
                        unsigned Component,
						double DotProduct) : Obstacle( SelfIndex,
                                                    getSourceIndex,
                                                    Component,
													DotProduct) {}

        virtual void treatBoundary( double * Field );
};


class MovingWall : public Obstacle {
    public:
        MovingWall( unsigned long int SelfIndex,
                    unsigned long int SourceIndex,
                    unsigned Component,
					double DotProduct) : Obstacle( SelfIndex,
                                                SourceIndex,
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
        Fluid( unsigned long int Coordinate ) : m_Coordinate( Coordinate ) {}

        void addObstacle( Obstacle* Obj );
        void processBoundary( double * Field );
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }
        unsigned long int getCoodinate() { return m_Coordinate; }

    private:
        unsigned long int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};

#endif
