#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <stdio.h>

#include <list>
#include "DataStructure.h"


void computePostCollisionDistributions( double *currentCell,
                                        const double * const tau,
                                        const double *const feq ) {


  double Inverse_Tau = 1.0 / ( *tau );

  for( int i = 0; i < Vel_DOF; ++i ) {
    currentCell[ i ] -= Inverse_Tau * ( currentCell[ i ] - feq[ i ]  );
  }

}

void doCollision( std::list<Fluid*>& FluidDomain,
				  double *collideField,
                  const double * const tau ) {

	//Variable declaration

  double Density = 0.0;
  double Velocity[ Dimensions ];
  double Feq[ Vel_DOF ];

  int Current_Cell = 0;

	//Looping through all fluid element
	for ( std::list<Fluid*>::iterator aFluidCell = FluidDomain.begin();
          aFluidCell != FluidDomain.end();
          ++aFluidCell ) {

		Current_Cell = (*aFluidCell)->getIndex(SELF_INDEX);

		computeDensity( ( collideField + Current_Cell ) , &Density );
		computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
		computeFeq( &Density , Velocity , Feq );
		computePostCollisionDistributions( ( collideField + Current_Cell ) , tau , Feq );

	}

}
