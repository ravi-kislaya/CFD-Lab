#include <iostream>
#include <string>
#include <stdlib.h>

#include "../DataStructure.h"
#include "../helper.h"

std::string TEST_HEADER = "\033[1;31mTEST: \033[0m";

void printPassedMessage( std::string FunctionName );

void printErrorMessage( std::string FunctionName );

template <class T, class D>
void printHintMessage( T Target, D ComputedData );

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


    BBInstance.setIndex( 1 );
    if ( BBInstance.generateProtocol() != -1 ) {
        printErrorMessage("generateProtocol() or setIndex()");
    }
    else {
        printPassedMessage( "guards of generateProtocol() and setIndex()" );
    }

    unsigned Length[ 3 ] = { 10, 10, 10 };
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

    BBInstance.setField(TestCollideField);
    if ( BBInstance.generateProtocol() == -1 ) {
        printErrorMessage("generateProtocol() or setField()");
    }
    else {
        printPassedMessage( "guards of generateProtocol() and setField()" );
    }


//..............................................................................




    // Fill in the buffer
    std::cout << std::endl;
    std::cout << "Test values:" << std::endl;
    for ( unsigned i = 0; i < TEST_SIZE; ++i ) {
         std::cout << i << ' ';
        BBInstance.addBufferElement( i*2 );
    }
    std::cout << std::endl;


    // Check the number of elements in the buffer
    if ( TEST_SIZE != BBInstance.getBufferSize() ) {
        printErrorMessage("getBufferSize()");
        printHintMessage( TEST_SIZE, BBInstance.getBufferSize() );
        abort();
    }
    else {
        printPassedMessage( "getBufferSize()" );
    }


    // try to generate protocol with bad parameters
    if ( TEST_SIZE != BBInstance.getBufferSize() ) {
        printErrorMessage("getBufferSize()");
        printHintMessage( TEST_SIZE, BBInstance.getBufferSize() );
        abort();
    }
    else {
        printPassedMessage( "getBufferSize()" );
    }



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
void printPassedMessage( std::string FunctionName ) {
    //system("color 01");
    std::cout << TEST_HEADER << FunctionName << " works OK..." << std::endl;
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
