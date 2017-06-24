#include <iostream>
#include <string>
#include <stdlib.h>

#include "../DataStructure.h"
#include "../helper.h"
#include "TESTvisual.h"

std::string TEST_HEADER = "\033[1;31mTEST: \033[0m";
const unsigned TRANSFERED_COMPONENTS_PER_LATTICE = 5;


void printPassedMessage( std::string FunctionName );
void printErrorMessage( std::string FunctionName );
template <class T, class D>
void printHintMessage( T Target, D ComputedData );
void InitXY( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer );

void InitXZ( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer );

void InitYZ( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer );




int main () {
    const unsigned TEST_SIZE = 10;

    BoundaryBuffer BBInstance;
//..............................  TEST  ........................................
    // check out all constructors
    if ( BBInstance.getProtocol() != 0 ) {
        printErrorMessage("Constructor - m_Protocol");
        printHintMessage( 0, BBInstance.getProtocol() );
    }
    else {
        printPassedMessage( "Constructor - m_Protocol and getProtocol()" );
    }


    if ( BBInstance.getField() != 0 ) {
        printErrorMessage("Constructor - m_Field");
        printHintMessage( 0, BBInstance.getField() );
    }
    else {
        printPassedMessage( "Constructor - m_Field and getField()" );
    }


    if ( BBInstance.getIndex() != -1 ) {
        printErrorMessage("Constructor - m_Index");
        printHintMessage( 0, BBInstance.getIndex() );
    }
    else {
        printPassedMessage( "Constructor - m_Index and getIndex()" );
    }

//..............................  TEST  ........................................
    // test whether the getProtocol guards work ok
    if ( BBInstance.generateProtocol() != -1 ) {
        printErrorMessage("generateProtocol()");
    }
    else {
        printPassedMessage( "guards of generateProtocol()" );
    }


    BBInstance.setIndex( 4 );
    if ( BBInstance.generateProtocol() != -1 ) {
        printErrorMessage("generateProtocol() or setIndex()");
    }
    else {
        printPassedMessage( "guards of generateProtocol() and setIndex()" );
    }

    unsigned Length[ 3 ] = { 10, 20, 30 };

    BBInstance.setDomainLength( Length );
    if ( BBInstance.generateProtocol() != -1 ) {
        printErrorMessage("generateProtocol() or setDomainLengths()");
    }
    else {
        printPassedMessage( "guards of generateProtocol() and setDomainLengths()" );
    }

     double *TestCollideField = 0;
     double *TestStreamField = 0;
     int *TestFlagField = 0;
     int *TestIdField = 0;
     allocateFields( &TestCollideField,
    	  			 &TestStreamField,
    				 &TestFlagField,
    				 &TestIdField,
    				 Length );

    BBInstance.setField( TestCollideField );

/*

    if ( BBInstance.generateProtocol() == -1 ) {
        printErrorMessage("generateProtocol() or setField()");
    }
    else {
        printPassedMessage( "guards of generateProtocol() and setField()" );
    }


//..............................  TEST  ........................................
    // CHECK Z direction
    //BBInstance.setIndex( 4 );
    //InitXY( TestCollideField, Length, 1, BBInstance );

    //BBInstance.setIndex( 5 );
    //InitXY( TestCollideField, Length, Length[ 2 ], BBInstance );

    //BBInstance.setIndex( 0 );
    //InitYZ( TestCollideField, Length, 1, BBInstance );

    //BBInstance.setIndex( 1 );
    //InitYZ( TestCollideField, Length, Length[ 0 ], BBInstance );

    //BBInstance.setIndex( 2 );
    //InitXZ( TestCollideField, Length, 1, BBInstance );

    BBInstance.setIndex( 3 );
    InitXZ( TestCollideField, Length, Length[ 1 ], BBInstance );


*/
    writeVtkOutputTest( TestCollideField,
                        "FieldBeforeTransfer",
                        0,
                        Length );
//..............................  TEST  ........................................
    BBInstance.generateProtocol();
    BBInstance.updateProtocol();

    double* Protocol = BBInstance.getProtocol();
    unsigned ProtocolSize = BBInstance.getProtocolSize();
    decodeProtocol( Protocol, ProtocolSize, TestCollideField );

    writeVtkOutputTest( TestCollideField,
                        "FieldAfterTransfer",
                        0,
                        Length );


    // delete flields
    free( TestCollideField );
    free( TestStreamField );
    free( TestFlagField );
    free( TestIdField );


    return 0;
}

//******************************************************************************
//                              HELPER FUNCTIONS
//******************************************************************************
void InitXY( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer ) {

    // Prepare a layer to be transfered
    unsigned Current_Field_Cell = 0;
    double TEST_VALUE = 777;
    for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
        for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

            Current_Field_Cell = computeFieldIndex( x, y, Offset, Length );
            for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
                Field[ Current_Field_Cell + i ] = TEST_VALUE;
            }

            aBuffer.addBufferElement( Current_Field_Cell );
        }
    }

    unsigned NumberOfTransferedComponents = TRANSFERED_COMPONENTS_PER_LATTICE
                                        * ( Length[1] + 2 )
                                        * ( Length[0] + 2 );

    if ( aBuffer.getBufferSize() != NumberOfTransferedComponents ) {
        printErrorMessage("getBufferSize() or addBufferElement()");
    }
    else {
        printPassedMessage( "getBufferSize() and addBufferElement()" );
    }

}

void InitXZ( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer ) {

    // Prepare a layer to be transfered
    unsigned Current_Field_Cell = 0;
    double TEST_VALUE = 777;
    for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
        for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

            Current_Field_Cell = computeFieldIndex( x, Offset, z, Length );
            for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
                Field[ Current_Field_Cell + i ] = TEST_VALUE;
            }

            aBuffer.addBufferElement( Current_Field_Cell );


        }
    }

    unsigned NumberOfTransferedComponents = TRANSFERED_COMPONENTS_PER_LATTICE
                                        * ( Length[1] + 2 )
                                        * ( Length[2] + 2 );

    if ( aBuffer.getBufferSize() != NumberOfTransferedComponents ) {
        printErrorMessage("getBufferSize() or addBufferElement()");
    }
    else {
        printPassedMessage( "getBufferSize() and addBufferElement()" );
    }

}

void InitYZ( double* Field,
             unsigned* Length,
             unsigned Offset,
             BoundaryBuffer& aBuffer ) {

    // Prepare a layer to be transfered
    unsigned Current_Field_Cell = 0;
    double TEST_VALUE = 777;
    for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
        for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {

            Current_Field_Cell = computeFieldIndex( Offset, y, z, Length );
            for( unsigned i = 0 ; i < Vel_DOF ; ++i ) {
                Field[ Current_Field_Cell + i ] = TEST_VALUE;
            }

            aBuffer.addBufferElement( Current_Field_Cell );

        }
    }

    unsigned NumberOfTransferedComponents = TRANSFERED_COMPONENTS_PER_LATTICE
                                        * ( Length[1] + 2 )
                                        * ( Length[0] + 2 );

    if ( aBuffer.getBufferSize() != NumberOfTransferedComponents ) {
        printErrorMessage("getBufferSize() or addBufferElement()");
    }
    else {
        printPassedMessage( "getBufferSize() and addBufferElement()" );
    }

}



void printPassedMessage( std::string FunctionName ) {
    //system("color 01");
    std::cout << TEST_HEADER << FunctionName << " work(s) OK..." << std::endl;
    //system("color 02");
}

void printErrorMessage( std::string FunctionName ) {
    std::cout << TEST_HEADER << "ERROR: in "<< FunctionName << "!!!" << std::endl;
}

template <class T, class D>
void printHintMessage( T Target, D ComputedData ) {
    std::cout << TEST_HEADER << "the value should be: " << Target << std::endl;
    std::cout << TEST_HEADER << "instead we have: " << ComputedData << std::endl;
}
