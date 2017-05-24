#include <list>


#ifndef _DATA_STRUCTURE_H_
#define _DATA_STRUCTURE_H_

//------------------------------------------------------------------------------
//                            Wall cell
//------------------------------------------------------------------------------
class Obstacle {
    public:
        Obstacle() : m_SelfIndex( 0 ), m_TargetIndex( 0 ) {}
        Obstacle( int SelfIndex, int getTargetIndex ) : m_SelfIndex( SelfIndex ),
                                                        m_TargetIndex( getTargetIndex ) {

                                                        }
        virtual ~Obstacle() {}
        virtual void treatBoundary() = 0;

        int getSelfIndex() { return m_SelfIndex; }
        int getTargetIndex() { return m_TargetIndex; }
    private:
        int m_SelfIndex;
        int m_TargetIndex;
};


class StationaryWall : public Obstacle {
    public:
        StationaryWall( int SelfIndex, int getTargetIndex )
                                    : Obstacle( SelfIndex, getTargetIndex ) {}

        virtual void treatBoundary() {};
};


class MovingWall : public Obstacle {
    public:
        MovingWall( int SelfIndex, int getTargetIndex, int Component )
                                    : Obstacle( SelfIndex, getTargetIndex ),
                                      m_VelocityComponent( Component ) {}

        virtual void treatBoundary() {};

    private:
        int m_VelocityComponent;
};



//------------------------------------------------------------------------------
//                            Fluid cell
//------------------------------------------------------------------------------
class Fluid {
    public:
        Fluid() : m_Coordinate( 0 ) {}

        void addObestacle( Obstacle* Obj );
        void processBoundary();
        void deleteObstacles();
        bool isEmpty() { return ObstacleList.empty(); }

    private:
        int m_Coordinate;
        std::list<Obstacle*> ObstacleList;

};

#endif
