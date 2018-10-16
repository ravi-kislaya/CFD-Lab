#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"

#include <stdio.h>
#include <vector>

#include "DataStructure.h"

void doCollision( std::vector<Fluid*>& FluidDomain,
				  double *collideField,
                  const double * const tau ) {

  double Inverse_Tau = 1.0 / ( *tau );

	//Looping through all fluid element

	for ( unsigned i = 0; i < FluidDomain.size(); ++i ) {

		FluidDomain[ i ]->doLocalCollision( collideField, Inverse_Tau );

	}

}
