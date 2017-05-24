#include <list>


#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_TargetIndex( 0 ) {}
        Obstacle( int SelfIndex,
                  int getTargetIndex,
                  int Component ) : m_SelfIndex( SelfIndex ),
                                    m_TargetIndex( getTargetIndex ),
                                    m_VelocityComponent( Component )  {}


        virtual ~Obstacle() {}
        virtual void treatBoundary( double * Field ) = 0;


        int getSelfIndex() { return m_SelfIndex; }
        int getTargetIndex() { return m_TargetIndex; }
    private:
        int m_SelfIndex;
        int m_TargetIndex;
        int m_VelocityComponent;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex,
                        int getTargetIndex,
                        int Component ) : Obstacle( SelfIndex,
                                                    getTargetIndex,
                                                    Component ) {}

        virtual void treatBoundary( double * Field );
};


class MovingWall : public Obstacle {
    public:
        MovingWall( int SelfIndex,
                    int getTargetIndex,
                    int Component ) : Obstacle( SelfIndex,
                                                getTargetIndex,
                                                Component ) {}

        virtual void treatBoundary( double * Field );
};



//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
class Fluid {
    public:
        Fluid() : m_Coordinate( 0 ) {}
        Fluid( int Coordinate ) : m_Coordinate( Coordinate ) {}

        void addObestacle( Obstacle* Obj );
        void processBoundary( double * Field );
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }
        int getCoodinate() { return m_Coordinate; }

    private:
        int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};

#endif
