#include <list>
#include <string>
#include <unordered_map>

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
                                                Component ) {
                        m_DotProduct = DotProduct;
                    }

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
		Fluid() :  m_ID( -1 ),
                   m_XCoord( 0.0 ),
                   m_YCoord( 0.0 ),
                   m_ZCoord( 0.0 ),
                   m_DiagonalLattice( 0 ),
                   m_CpuID( 0 ) {

			for( int i = 0; i < Vel_DOF; ++i ) {
				m_NeighbourIndex[ i ] = 0;
                m_GloabalNeighbourIndex[ i ] = 0;
			}

		}

		Fluid( int ID, int x, int y, int z, int* Index ) :  m_ID( ID ),
                                                            m_XCoord( x ),
                                                            m_YCoord( y ),
                                                            m_ZCoord( z ),
                                                            m_DiagonalLattice( 0 ),
                                                            m_BoundaryTag("NONE"),
                                                            m_CpuID( 0 ) {
            // Assign all neighbours
			for( int i = 0; i < Vel_DOF; ++i ) {
				m_NeighbourIndex[ i ] = Index[ i ];
			}
		}

        // GETTER FUNCTIONS
        int getID() { return m_ID; }
        double getXCoord() { return m_XCoord; }
        double getYCoord() { return m_YCoord; }
        double getZCoord() { return m_ZCoord; }
        int getIndex( int Index ) { return m_NeighbourIndex[ Index ]; }
        void doLocalStreaming( double* collideField, double* streamField );
		void doLocalCollision( double *collideField, double Inverse_Tau );
        int getDiagonalLattice() { return (int)( m_DiagonalLattice ); }
        std::string getBoundaryTag() { return m_BoundaryTag; }
        int getCpuID() { return m_CpuID; }

        int getIdIndex( int Index ) { return (int)( m_NeighbourIndex[ Index ] ); }
        int getGloalIdIndex( int Index ) { return (int)( m_GloabalNeighbourIndex[ Index ] ); }

        // STTER FUNCTIONS
        void setID( int ID ) { m_ID = ID; }
        void setXCoord( double XCoord ) { m_XCoord = XCoord; }
        void setYCoord( double YCoord ) { m_YCoord = YCoord; }
        void setZCoord( double ZCoord ) { m_ZCoord = ZCoord; }
        void setDiagonalLattice( int ID ) { m_DiagonalLattice = ID; }
        void setBoundaryTag( std::string Tag ) { m_BoundaryTag = Tag; }
        void setCpuID( int CpuID ) { m_CpuID = CpuID; }

        void setIndex( int NeighborID, int Index ) {
            m_NeighbourIndex[ Index ] = NeighborID;
        }

        void setGloabalIndex( int GlobalNeighborID, int Index ) {
            m_GloabalNeighbourIndex[ Index ] = GlobalNeighborID;
        }

	private:
        int m_ID;
        double m_XCoord;
        double m_YCoord;
        double m_ZCoord;
        int m_NeighbourIndex[ Vel_DOF ];
        int m_GloabalNeighbourIndex[ Vel_DOF ];
        int m_DiagonalLattice;
        std::string m_BoundaryTag;
        int m_CpuID;
};



class BoundaryEntry {
    public:
        BoundaryEntry() : Name( "NONE" ),
                          BoundaryID( -1 ),
                          TYPE( -1 )  {
            for ( int i = 0; i < Dimensions; ++i  ) {
                Data[ i ] = -1.0;
            }
        }

        std::string Name;
        int BoundaryID;
        int TYPE;
        double Data[ Dimensions ];
};


//------------------------------------------------------------------------------
//                            Boundary Buffer
//------------------------------------------------------------------------------
//scheme :
class BoundaryBuffer {
	public:
		BoundaryBuffer();
        ~BoundaryBuffer();


        // Getter FUNCTIONS
        double* getField() { return m_Field; };
		double* getProtocol();
        unsigned getBufferSize() { return (unsigned)BufferElements.size(); };
        unsigned getProtocolSize() { return 2 * (unsigned)BufferElements.size(); };
        int getIndex() { return  m_Index; };
        int getTragetCpu() { return m_TragetCpu; }


		void addBufferElement( unsigned Index );
		int updateProtocol();
		int generateProtocol( std::unordered_map<unsigned, unsigned>& LocalToGlobalIdTable );


        // Setter FUNCTIONS
        void setIndex( unsigned Index ) { m_Index = Index; };
        void setTragetCpu( int TragetCpuId ) { m_TragetCpu = TragetCpuId; }
        void setField ( double* Field ) { m_Field = Field; }

	private:
		std::list<unsigned> BufferElements;
		double* m_Protocol;
        double* m_Field;
        int m_Index;
        unsigned m_BufferSize;
        bool m_isProtocolReady;
        int m_TragetCpu;
};


void decodeProtocol( double* Protocol,
                     unsigned ProtocolSize,
                     double* Field,
                     std::unordered_map<unsigned, unsigned>& GlobalToLocalIdTable );



#endif
