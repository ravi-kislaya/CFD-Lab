#include "streaming.h"
#include "LBDefinitions.h"
#include "helper.h"

void doStreaming( double *collideField,
	 			  double *streamField,
				  std::list<Fluid*>& FluidDomain ) {


	int Fluid_Cell = 0, Neighbour_Cell = 0;

	//Looping through all fluid element
	for ( std::list<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {
			  
		int Fluid_Cell = (*aFluidCell)->getIndex(SELF_INDEX);

		//Cell wise streaming considering the velocity component
		for( int Vel_Component = 0; Vel_Component < Vel_DOF; ++Vel_Component ) {


			Neighbour_Cell = (*aFluidCell)->getIndex(Vel_Component);

			streamField[ Fluid_Cell + 18 - Vel_Component ]
							= collideField[ Neighbour_Cell + 18 - Vel_Component ] ;
		
		}
	}
}
