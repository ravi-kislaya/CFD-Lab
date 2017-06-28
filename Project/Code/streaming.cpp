#include "streaming.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <vector>
#include "DataStructure.h"

void doStreaming( double *collideField,
	 			  double *streamField,
				  std::vector<Fluid*>& FluidDomain ) {


	int Fluid_Cell = 0, Neighbour_Cell = 0;

	//Looping through all fluid element
	for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

		(*aFluidCell)->doLocalStreaming( collideField, streamField );

	}
}
