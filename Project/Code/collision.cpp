#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <stdio.h>

#include <vector>
#include "DataStructure.h"

void doCollision( std::vector<Fluid*>& FluidDomain,
				  double *collideField,
                  const double * const tau ) {

	//Variable declaration

/*  double *Density;
  *Density = 0.0;
  double Velocity[ Dimensions ] = { 0.0 };
  double Feq[ Vel_DOF ] = { 0.0 };*/
  double Inverse_Tau = 1.0 / ( *tau );

	//Looping through all fluid element
	for ( std::vector<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

		(*aFluidCell)->doLocalCollision( collideField, Inverse_Tau );

	}

}
