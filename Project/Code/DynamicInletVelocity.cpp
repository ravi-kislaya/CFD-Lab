#include "DynamicInletVelocity.h"
#include "helper.h"
#include "initLB.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

void readInletVelocity( std::vector<double> &TimeVelocity,
						int &LBStep,
						int &EndTimeStep ) {
							
	std::string BufferString;
    std::list<std::string> BufferList;
    std::ifstream FILE( "./InletVelocity/aorta.inl" );
     while( !FILE.eof() ) {
         std::getline( FILE, BufferString );
         BufferList.push_back( BufferString );
     }
     FILE.close();
	std::list<std::string>::iterator aString = BufferList.begin();
	
	double OneTimeStep = findDoubleInString( *(aString++) );
	LBStep = (int) (findDoubleInString( *(aString++) ) / OneTimeStep);
	EndTimeStep = (int) (findDoubleInString( *(aString++) ) / OneTimeStep);

    // remove the header and tail from the buffer
    trimList( BufferList );

	for ( std::list<std::string>::iterator aString = BufferList.begin();
          aString != BufferList.end();
          ++aString ) {
		TimeVelocity.push_back( std::stod(*aString) );
	}
	
}

void interpolateInletVelocity(  double &NewVelocity,
								std::vector<double> &TimeVelocity,
								int TimeStep,
								int LBStep,
								int EndTimeStep  ) {
	
	int RemainderTime = TimeStep % EndTimeStep;
	int LowerBound = (int) RemainderTime / LBStep;
	NewVelocity = TimeVelocity[LowerBound] 
				+ (( TimeVelocity[LowerBound+1] - TimeVelocity[LowerBound] )
				 *  RemainderTime / LBStep );
	
}