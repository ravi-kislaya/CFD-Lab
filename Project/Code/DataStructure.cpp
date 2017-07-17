#include "DataStructure.h"
#include "helper.h"
#include "computeCellValues.h"
#include "LBDefinitions.h"
#include <stdio.h>
#include <iostream>
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

	unsigned  Reflected_Vel_Component[5] = { 0 };
	unsigned Source_Vel_Component[5] = { 0 };

	if(m_VelocityComponent == 2) {

		Reflected_Vel_Component[ 0 ] = 16;
		Reflected_Vel_Component[ 1 ] = 18;
		Reflected_Vel_Component[ 2 ] = 14;
		Reflected_Vel_Component[ 3 ] = 17;
		Reflected_Vel_Component[ 4 ] = 15;


		Source_Vel_Component[ 0 ] = 2;
		Source_Vel_Component[ 1 ] = 4;
		Source_Vel_Component[ 2 ] = 0;
		Source_Vel_Component[ 3 ] = 3;
		Source_Vel_Component[ 4 ] = 1;

	}

	else if(m_VelocityComponent == 6) {

		Reflected_Vel_Component[ 0 ] = 12;
		Reflected_Vel_Component[ 1 ] = 18;
		Reflected_Vel_Component[ 2 ] = 4;
		Reflected_Vel_Component[ 3 ] = 13;
		Reflected_Vel_Component[ 4 ] = 11;


		Source_Vel_Component[ 0 ] = 6;
		Source_Vel_Component[ 1 ] = 14;
		Source_Vel_Component[ 2 ] = 0;
		Source_Vel_Component[ 3 ] = 7;
		Source_Vel_Component[ 4 ] = 5;

	}

	else if(m_VelocityComponent == 8) {

		Reflected_Vel_Component[ 0 ] = 10;
		Reflected_Vel_Component[ 1 ] = 13;
		Reflected_Vel_Component[ 2 ] = 7;
		Reflected_Vel_Component[ 3 ] = 17;
		Reflected_Vel_Component[ 4 ] = 3;


		Source_Vel_Component[ 0 ] = 8;
		Source_Vel_Component[ 1 ] = 11;
		Source_Vel_Component[ 2 ] = 5;
		Source_Vel_Component[ 3 ] = 15;
		Source_Vel_Component[ 4 ] = 1;

	}

	else if(m_VelocityComponent == 10) {

		Reflected_Vel_Component[ 0 ] = 8;
		Reflected_Vel_Component[ 1 ] = 11;
		Reflected_Vel_Component[ 2 ] = 5;
		Reflected_Vel_Component[ 3 ] = 15;
		Reflected_Vel_Component[ 4 ] = 1;


		Source_Vel_Component[ 0 ] = 10;
		Source_Vel_Component[ 1 ] = 13;
		Source_Vel_Component[ 2 ] = 7;
		Source_Vel_Component[ 3 ] = 17;
		Source_Vel_Component[ 4 ] = 3;

	}

	else if(m_VelocityComponent == 12) {

		Reflected_Vel_Component[ 0 ] = 6;
		Reflected_Vel_Component[ 1 ] = 0;
		Reflected_Vel_Component[ 2 ] = 14;
		Reflected_Vel_Component[ 3 ] = 7;
		Reflected_Vel_Component[ 4 ] = 5;


		Source_Vel_Component[ 0 ] = 12;
		Source_Vel_Component[ 1 ] = 4;
		Source_Vel_Component[ 2 ] = 18;
		Source_Vel_Component[ 3 ] = 13;
		Source_Vel_Component[ 4 ] = 11;

	}

	else {

		Reflected_Vel_Component[ 0 ] = 2;
		Reflected_Vel_Component[ 1 ] = 4;
		Reflected_Vel_Component[ 2 ] = 0;
		Reflected_Vel_Component[ 3 ] = 3;
		Reflected_Vel_Component[ 4 ] = 1;


		Source_Vel_Component[ 0 ] = 16;
		Source_Vel_Component[ 1 ] = 18;
		Source_Vel_Component[ 2 ] = 14;
		Source_Vel_Component[ 3 ] = 17;
		Source_Vel_Component[ 4 ] = 15;
	}

	for(int i=0; i < 5; ++i) {
		Field[ m_SelfIndex + Reflected_Vel_Component[i] ]
						= Field[ m_SourceIndex + Source_Vel_Component[i] ];
	}

}

/********************Inflow************************************/
inline void Inflow::treatBoundary( double * Field ) {

	double Density = 1.0;
 	double TempF = 0.0;
	//Debugging begin
	//printf("VX =  %f, VY =%f, VZ =%f \n",m_InletVelocity[0],m_InletVelocity[1],m_InletVelocity[2]);
	//Debugging end
 	computeDensity( Field + m_SourceIndex, &Density );
 	computeSingleFeq( &Density, m_InletVelocity, &TempF, m_VelocityComponent );
 	Field[m_SelfIndex + m_VelocityComponent] = TempF;

}

/********************PressureIn************************************/
inline void PressureIn::treatBoundary( double * Field ) {

	double Density = Density_Reference + m_DeltaDensity;

	double VelocityNeighbourFluid[ Dimensions ] = {0.0};
	double FeqI = 0.0;
	double FeqInvI = 0.0;
	double TempDensity = 0.0;

	computeDensity( Field + m_SourceIndex, &TempDensity );
	computeVelocity( Field+m_SourceIndex, &TempDensity, VelocityNeighbourFluid );
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


//------------------------------------------------------------------------------
//                             Fluid cell
//------------------------------------------------------------------------------
/*********************************Streaming************************************/
void Fluid::doLocalStreaming( double* collideField, double* streamField ) {

	int Current_Cell = Vel_DOF * m_NeighbourIndex[ SELF_INDEX ];
	int Neighbour_Cell = 0;
	for( int Vel_Component = 0; Vel_Component < Vel_DOF; ++Vel_Component ) {
		Neighbour_Cell = Vel_DOF * m_NeighbourIndex [ Vel_Component ];
		streamField[ Current_Cell + 18 - Vel_Component ]
			= collideField[ Neighbour_Cell + 18 - Vel_Component ];

	}
}

/*********************************Collision************************************/
void Fluid::doLocalCollision( double *collideField,
							  double Inverse_Tau ) {

	int Current_Cell = Vel_DOF * m_NeighbourIndex[ SELF_INDEX ];
	double Density = 0.0;
	double Velocity[ Dimensions ] = { 0.0 };
	double Feq[ Vel_DOF ] = { 0.0 };

	computeDensity( ( collideField + Current_Cell ) , &Density );
	computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
	computeFeq( &Density , Velocity , Feq );
	for( int i = 0; i < Vel_DOF; ++i ) {
		collideField[ Current_Cell + i ] -= Inverse_Tau
										  * ( collideField[ Current_Cell + i ] - Feq[ i ]  );
	}

}



//------------------------------------------------------------------------------
//                            Boundary Buffer
//------------------------------------------------------------------------------
BoundaryBuffer::BoundaryBuffer() : m_Protocol( 0 ),
								   m_Indices( 0 ),
								   m_ReceivedProtocol( 0 ),
								   m_Field( 0 ),
 								   m_BufferSize( 0 ),
 								   m_isProtocolReady( false ),
 								   m_TragetCpu( -1 ) {

}

BoundaryBuffer::~BoundaryBuffer() {

	if ( m_Protocol != 0 ) {
		delete [] m_Protocol;
		delete [] m_ReceivedProtocol;
        delete [] m_Indices;
	}

}


BoundaryBuffer::BoundaryBuffer( const BoundaryBuffer& aBuffer ) {

	this->m_BufferElements = aBuffer.m_BufferElements ;
    this->m_BufferSize = aBuffer.m_BufferSize;
    this->m_isProtocolReady = aBuffer.m_isProtocolReady;
    this->m_TragetCpu = aBuffer.m_TragetCpu;
    this->m_Field = aBuffer.m_Field;

	if ( aBuffer.m_BufferSize != 0 ) {

		this->m_Protocol = new double[ m_BufferSize ];
		this->m_ReceivedProtocol = new double[ m_BufferSize ];
        this->m_Indices = new int[ m_BufferSize ];

		for ( unsigned i = 0; i < m_BufferSize; ++i ) {
			this->m_Protocol[ i ] = aBuffer.m_Protocol[ i ];
			this->m_ReceivedProtocol[ i ] = aBuffer.m_ReceivedProtocol[ i ];
            this->m_Indices[ i ] = aBuffer.m_Indices[ i ];
		}
	}

}


const BoundaryBuffer* BoundaryBuffer::operator=( const BoundaryBuffer& aBuffer ) {

	// Free memory on the heap if the protocol and receive buffer were generated
	// calling the assignment operator before
	if ( this->m_Protocol != 0 ) {
		delete [] m_Protocol;
		delete [] m_ReceivedProtocol;
        delete [] m_Indices;
	}

	this->m_BufferElements = aBuffer.m_BufferElements ;
    this->m_BufferSize = aBuffer.m_BufferSize;
    this->m_isProtocolReady = aBuffer.m_isProtocolReady;
    this->m_TragetCpu = aBuffer.m_TragetCpu;
    this->m_Field = aBuffer.m_Field;

	if ( aBuffer.m_BufferSize != 0 ) {

		this->m_Protocol = new double[ m_BufferSize ];
		this->m_ReceivedProtocol = new double[ m_BufferSize ];
		this->m_Indices = new int[ m_BufferSize ];

		for ( unsigned i = 0; i < this->m_BufferSize; ++i ) {
			this->m_Protocol[ i ] = aBuffer.m_Protocol[ i ];
			this->m_ReceivedProtocol[ i ] = aBuffer.m_ReceivedProtocol[ i ];
            this->m_Indices[ i ] = aBuffer.m_Indices[ i ];
		}
	}

	return this;
}

void BoundaryBuffer::flushBuffer() {
	if ( this->m_Protocol != 0 ) {
		delete [] m_Protocol;
		delete [] m_ReceivedProtocol;
        delete [] m_Indices;
	}

	m_BufferElements.clear();
	m_TragetCpu = -1;

}

// inline DEBUGGING
void BoundaryBuffer::addBufferElement( unsigned Index ) {
	m_BufferElements.push_back( Index );
}


void BoundaryBuffer::finalizeMapping( int* ReceivedIndicies, 
				std::unordered_map<unsigned, unsigned>& GlobalToLocalIdTable ) {

	unsigned LocalFiledID = 0;
	unsigned GlobalFiledID = 0;
	unsigned Shift = 0;
	std::unordered_map<unsigned, unsigned>::const_iterator IdIterator;

    for ( unsigned i = 0; i < m_BufferSize; ++i ) {

		GlobalFiledID = ReceivedIndicies[ i ] / Vel_DOF;
		Shift = ReceivedIndicies[ i ] - Vel_DOF * ( GlobalFiledID );

		IdIterator = GlobalToLocalIdTable.find( GlobalFiledID );
		LocalFiledID = IdIterator->second;
		LocalFiledID = Vel_DOF * LocalFiledID + Shift;

        m_Indices[ i ] = LocalFiledID;

	}
}

double* BoundaryBuffer::getProtocol() {
    this->updateProtocol();
    return m_Protocol;
}


int BoundaryBuffer::initializeMapping( std::unordered_map<unsigned, unsigned>& LocalToGlobalIdTable ) {

// ............................ GUARDS: BEGINING ...............................

	if ( m_TragetCpu == -1 ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing the target cpu" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


	if ( m_Field == 0 ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing the field" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


	if ( LocalToGlobalIdTable.size() == 0 ) {
		std::cout << "\tERROR: you've tried to generate the protocol" << std::endl;
		std::cout << "\twithout initializing the MappingTable" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> generateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}



// .............................. GUARDS: END .................................

	m_BufferSize = this->getBufferSize();
	if ( this->getBufferSize() != 0 ) {
		if ( m_Protocol != 0 ) {
			delete [ ] m_Protocol;
            delete [ ] m_ReceivedProtocol;
            delete [ ] m_Indices;
        }
		m_Protocol = new double[ m_BufferSize ];
		m_ReceivedProtocol = new double[ m_BufferSize ];
        m_Indices = new int[ m_BufferSize ];
	}


	unsigned LocalFiledID = 0;
	unsigned GlobalFiledID = 0;
	unsigned Shift = 0;
	std::unordered_map<unsigned, unsigned>::const_iterator IdIterator;
	unsigned Counter = 0;


    for ( std::list<unsigned>::iterator Iterator = m_BufferElements.begin();
 		  Iterator != m_BufferElements.end();
		  ++Iterator, ++Counter ) {

				// Iterator is given as the local field + shift. We need to
				// convert it to global diled id + shift for the communication
				// purposes
				LocalFiledID = ( *Iterator ) / Vel_DOF;
				Shift = ( *Iterator ) - Vel_DOF * LocalFiledID;

				IdIterator = LocalToGlobalIdTable.find( LocalFiledID );
				GlobalFiledID = IdIterator->second;
				GlobalFiledID = GlobalFiledID * Vel_DOF + Shift;

                m_Indices[ Counter ] = GlobalFiledID;

    }


	m_isProtocolReady = true;

	return 1;
}



int  BoundaryBuffer::updateProtocol() {

	if ( m_isProtocolReady == false ) {
		std::cout << "\tERROR: you've tried to update the protocol" << std::endl;
		std::cout << "\twhich was not be generated" << std::endl;
		std::cout << "\tERROR SOURCE: DataStructure.cpp -> updateProtocol()" << std::endl;
#ifdef TEST
		return -1;
#else
		exit( EXIT_FAILURE ); // DEBUGGING
#endif
	}


	unsigned Counter = 0;
	for ( std::list<unsigned>::iterator Iterator = m_BufferElements.begin();
 		  Iterator != m_BufferElements.end();
		  ++Iterator, ++Counter ) {

	      m_Protocol[ Counter ] = m_Field[ ( *Iterator ) ];
	}

	return 0;
}


void BoundaryBuffer::printBufferElements() {

    for ( auto Iterator = m_BufferElements.begin();
          Iterator != m_BufferElements.end();
          ++Iterator) {

        std::cout << (*Iterator) << std::endl;
    }

}


void BoundaryBuffer::printProtocol() {

    for ( unsigned i = 0; i < m_BufferElements.size(); ++i ) {
        std::cout << (unsigned)( m_Protocol[ i ] ) << std::endl;
    }

}


void BoundaryBuffer::unpackReceiveProtocol( double* Field ) {

    for ( unsigned i = 0; i < m_BufferSize; ++i ) {

        Field[ m_Indices[ i ] ] = m_ReceivedProtocol[ i ]; 

    } 
}
