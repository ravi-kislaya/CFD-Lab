#include "streaming.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <vector>
#include "DataStructure.h"

void doStreaming( double *collideField,
	 			  double *streamField,
				  std::vector<Fluid*>& FluidDomain ) {

	//Looping through all fluid element
	for ( unsigned i = 0; i < FluidDomain.size(); ++i ) {

		FluidDomain[ i ]->doLocalStreaming( collideField, streamField );

	}
}
