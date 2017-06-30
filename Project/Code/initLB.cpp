#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>

#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include "DataStructure.h"

/*******************************************************************************
                        DECLARATION OF HELPER FUNCTIONS: START
*******************************************************************************/
template< class T >
void initializeFiledByZero( T* Array, int SIZE );

std::vector<std::string> getVectorOfStrings( std::string &aString );
int findIntegerInString ( std::string &aString );
int convertBoundaryStringToInteger( std::string &aString );
std::vector<BoundaryEntry*> converToBoundary( std::list<std::string> &aList );
void freeListMemory( std::list<BoundaryEntry*> &aList );
void freeVectorMemory( std::vector<BoundaryEntry*> &aVector );
void trimList( std::list<std::string> &aList );
/*******************************************************************************
                        DECLARATION OF HELPER FUNCTIONS: END
*******************************************************************************/



/*******************************************************************************
                    DATA INITIALIZATION AND FILE PROCESSING
*******************************************************************************/
void initialiseData( double** collideField,
                     double** streamField,
                     int** flagField,
                     int** VtkID,
                     std::vector<Fluid*> &FluidDomain,
                     std::vector<BoundaryEntry*> &BoundaryConditions ) {


//....................... PROCESS FLAG FIELD FILE ..............................
    // *** READ BOUNDARY LIST
    std::string BufferString;
    std::list<std::string> BufferList;


    std::ifstream FILE( "./Mesh/BoundaryList.bc" );
     while( !FILE.eof() ) {
         std::getline( FILE, BufferString );
         BufferList.push_back( BufferString );
     }
     FILE.close();

    // remove the header and tail from the buffer
    trimList( BufferList );

    // generate Boundary Element container
    BoundaryConditions = converToBoundary( BufferList );


    // generare a hash table for the boundaries
    std::unordered_map<std::string, int> LookUpTable;
    for ( unsigned i = 0; i < BoundaryConditions.size(); ++i ) {
        LookUpTable.insert( { BoundaryConditions[ i ]->Name,
                              BoundaryConditions[ i ]->BoundaryID } );

    }



    // *** READ FLAG FIELD
     BufferList.clear();
     FILE.open( "./Mesh/FlagFields.fg" );

     while( !FILE.eof() ) {
         std::getline( FILE, BufferString );
         BufferList.push_back( BufferString );
     }
     FILE.close();


    // Find out the size of the problem by reading the first string
    int ProblemSize = findIntegerInString( *BufferList.begin() );

    // Trow an error if the ProblemSize is meaningful
    if ( ProblemSize == 0  ) {
        std::string ERROR = "ERROR: the size of the problem is a negative value\n";
        ERROR += "SOURCE of the error: initialiseData(...)";
        throw ERROR;
    }


    ( *flagField ) = new int[ ProblemSize ];
    ( *VtkID ) = new int[ ProblemSize ];
    initializeFiledByZero( ( *flagField ), ProblemSize );
    initializeFiledByZero( ( *VtkID ), ProblemSize );

    // remove the header and tail from the buffer
    trimList( BufferList );



     // Fill in the flag field according to the boundary name of each lattice
    std::vector<std::string> BufferVector;
    std::unordered_map<std::string, int>::const_iterator LookUpTableIterator;
    int BoundaryID = -1;
     for ( std::list<std::string>::iterator aString = BufferList.begin();
           aString != BufferList.end();
           ++aString ) {

         // parse a string obtained from the Flag Field file
         BufferVector = getVectorOfStrings( *aString );

         // Find the appropriate BoundaryID based on the boundary name
         LookUpTableIterator = LookUpTable.find( BufferVector[ 1 ] );
         BoundaryID = LookUpTableIterator->second;


        // DEBUGGING: print out the boundary name and assigned boundary ID
        //std::cout << BufferVector[ 1 ] << " == " << BoundaryID << std::endl;


        ( *flagField )[ std::stoi( BufferVector[ 0 ] ) ] = BoundaryID;
    }



//......................... PROCESS NEIGHBORS FILE  ............................
    // *** READ NEIGHBORS LIST
     BufferList.clear();
     FILE.open( "./Mesh/Neighbors.nb" );

     while( !FILE.eof() ) {
         std::getline( FILE, BufferString );
         BufferList.push_back( BufferString );
     }
     FILE.close();

    trimList( BufferList );

    Fluid* aFluidElement = 0;
    // go through the entire list and fill in FLuid Domain list
    for ( std::list<std::string>::iterator aString = BufferList.begin();
          aString != BufferList.end();
          ++aString ) {

        // parse a string obtained from the Neighbors file
        BufferVector = getVectorOfStrings( *aString );

         // Check whether an entry has neighbors or not. Skip the line
         // if a lattice cell has no neighbors.
         // NOTE: "NONE" means that a lattice is not a fluid element
         // IMPORTANT: according to the format the first entry in a string
         // is the Cell ID. The second entry is either the first neighbor
         // or indicator that tells that a lattice is not a fluid element
         // The last elemet is the reference to the corner element (+x,+y,+z)
         // that is necessary for vtk files
        if ( BufferVector[ 1 ] == "NONE" )
            continue;

        // create and initialize a fluid element by the parsed string
        aFluidElement = new Fluid;
        aFluidElement->setID( std::stoi( BufferVector[ 0 ] ) );
        int i = 0;
        for ( i = 1; i < Vel_DOF + 1; ++i ) {
            aFluidElement->setIndex( std::stoi( BufferVector[ i ] ), i - 1 );
        }
        aFluidElement->setDiagonalLattice( std::stoi( BufferVector[ i ] ) );

        // DEBUGGING: print out the cell ID, 10th element and the diagonal element
        // std::cout << aFluidElement->getID() << " == "
        //           << aFluidElement->getIdIndex( SELF_INDEX ) << " DIAGONAL: "
        //           <<  aFluidElement->getDiagonalLattice() << std::endl;

        // add the created fluid element to the vector
        FluidDomain.push_back( aFluidElement );
    }


//...................... PROCESS FILE OF COORDINATES  ..........................
    // *** READ NEIGHBORS LIST
     BufferList.clear();
     FILE.open( "./Mesh/Coordinates.crd" );

     while( !FILE.eof() ) {
         std::getline( FILE, BufferString );
         BufferList.push_back( BufferString );
     }
     FILE.close();

    trimList( BufferList );


    // convert the trimed list to a vector
    std::vector<std::string> CoordinatesVector;

    for ( std::list<std::string>::iterator Iterator = BufferList.begin();
          Iterator != BufferList.end(); ) {
        CoordinatesVector.push_back( *Iterator );
        BufferList.erase( Iterator++ );
    }

    // go through the entire list and assign coordinates to fluid lattices
    for ( unsigned i = 0; i < FluidDomain.size(); ++ i ) {


        // find an entry in the Coordinates file by the lattice ID
        // and parse that string
        BufferVector
            = getVectorOfStrings( CoordinatesVector[ FluidDomain[ i ]->getID() ] );

        // assign the coordinates that the contains to the lattice object
        FluidDomain[ i ]->setXCoord( std::stod( BufferVector[ 1 ] ) );
        FluidDomain[ i ]->setYCoord( std::stod( BufferVector[ 2 ] ) );
        FluidDomain[ i ]->setZCoord( std::stod( BufferVector[ 3 ] ) );

        // DEBUGGING:
//        std::cout << FluidDomain[ i ]->getID() << "::  "
//                  << FluidDomain[ i ]->getXCoord() << "  "
//                  << FluidDomain[ i ]->getYCoord() << "  "
//                  << FluidDomain[ i ]->getZCoord() << "  "
//                  << std::endl;

    }

    //...................... COMPLETE INITIALIZATION  ..........................

    int FieldSize = Vel_DOF * ProblemSize;
    // allocate collide and stream fields
    ( *collideField ) = new double [ FieldSize ];
    ( *streamField  ) = new double [ FieldSize ];

    // assign initial values for the fields
    // go through all elements in the flag and collide fields
    for ( int i = 0; i < FieldSize; i += Vel_DOF ) {

        // assign all 19 components of each lattice with the corresponding values
        for( int Component = 0; Component < Vel_DOF; ++Component ) {
            ( *collideField )[ i + Component ] = LATTICEWEIGHTS[ Component ];
            ( *streamField )[ i + Component ] = LATTICEWEIGHTS[ Component ];
        }

    }
}






/*******************************************************************************
                        INITIALIZATION HELPER FUNCTIONS
*******************************************************************************/
std::vector<std::string> getVectorOfStrings( std::string &aString ) {

    const char SPACE_DELIMITER = ' ';
    const char TAB_DELIMITER = '\t';
    std::string WordBuffer;

    std::vector<std::string> Words;

    // split a string into words and put each word into the buffer
    for( unsigned i = 0; aString[ i ]; ++i  ) {
        if( ( aString[ i ] != SPACE_DELIMITER )
            && ( aString[ i ] != TAB_DELIMITER ) ) {
            WordBuffer += aString[ i ];
        }
        else{
            Words.push_back( WordBuffer );
            WordBuffer = "";
        }
    }
    // process a string buffer if it's not empty at the end of the for - loop
    if ( !WordBuffer.empty() ) {
        Words.push_back( WordBuffer );
    }

    return Words;
}



int findIntegerInString ( std::string &aString ) {

    int Result = -1;
    std::vector<std::string> Words = getVectorOfStrings(aString);

    for ( unsigned i = 0; i < Words.size(); ++i ) {

        try{
            // try to convert a word to an integer
            Result = std::stoi( Words[ i ] );
        }
        catch( std::exception const & ERROR ) {

            // Continue if it's not possible to convert an instance to an integer
            continue;
        }

        // if we reach that place it means that we've found the first
        // integer in the given string
        break;
    }

    return Result;
}

template< class T >
void initializeFiledByZero( T* Array, int SIZE ) {

    const int DEFAULT_VALUE = -1;
    for ( int i = 0; i < SIZE; ++i ) {
        Array[ i ] = DEFAULT_VALUE;
    }
}

int convertBoundaryStringToInteger( std::string &aString ) {

    if ( aString == "FLUID" ) {
        return 0;
    }
    if ( aString == "NO_SLIP" ) {
        return 1;
    }
    if ( aString == "MOVING_WALL" ) {
        return 2;
    }
    if ( aString == "FREE_SLIP" ) {
        return 3;
    }
    if ( aString == "INFLOW" ) {
        return 4;
    }
    if ( aString == "PRESSURE_IN" ) {
        return 5;
    }
    if ( aString == "OUTFLOW" ) {
        return 6;
    }

    return -1;
}


std::vector<BoundaryEntry*> converToBoundary( std::list<std::string> &aList ) {

    std::vector<BoundaryEntry*> BoundaryList;
    std::vector<std::string> ParsedString;
    BoundaryEntry* Dummy;

     int BoundaryID = 0;
     for ( std::list<std::string>::iterator aString = aList.begin();
           aString != aList.end();
           ++aString, ++BoundaryID ) {

            ParsedString = getVectorOfStrings( *aString );

            Dummy = new BoundaryEntry;
            Dummy->Name = ParsedString[ 0 ];
            Dummy->BoundaryID = BoundaryID;
            Dummy->TYPE = convertBoundaryStringToInteger( ParsedString[ 1 ] );


            if ( ( Dummy->TYPE == INFLOW ) || ( Dummy->TYPE == MOVING_WALL ) ) {
                Dummy->Data[ 0 ] = std::stoi( ParsedString[ 2 ] );
                Dummy->Data[ 1 ] = std::stoi( ParsedString[ 3 ] );
                Dummy->Data[ 2 ] = std::stoi( ParsedString[ 4 ] );
            }

            BoundaryList.push_back( Dummy );
            ParsedString.clear();
     }

    return BoundaryList;
}


void freeListMemory( std::list<BoundaryEntry*> &aList ) {

     for ( std::list<BoundaryEntry*>::iterator Entry = aList.begin();
           Entry != aList.end();
           ++Entry ) {

        delete ( *Entry );

     }
}


void freeVectorMemory( std::vector<BoundaryEntry*> &aVector ) {

     for ( unsigned i = 0; i < aVector.size(); ++i ) {
        delete aVector[ i ];
     }
}


void trimList( std::list<std::string> &aList ) {

    // Delete all entries before delimiter
    std::list<std::string>::iterator Entry = aList.begin();
    while ( (*Entry)[ 0 ] != FILE_DELIMITER ) {
        aList.erase( Entry++ );
    }

    // remove delimiter from the list
    aList.erase( Entry++ );

    // remove the last element of the list
    // IMPORTANT: the last element is usually an empty string
    Entry = --aList.end();
    if ( (*Entry).size() == 0 ) {
        aList.erase( Entry );
    }

}
