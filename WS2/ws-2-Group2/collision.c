#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <stdio.h>


void computePostCollisionDistributions( double *currentCell,
                                        const double * const tau,
                                        const double *const feq ) {

  double Inverse_Tau = 1.0 / ( *tau );
//Solves Equation 13
  for( unsigned i = 0; i < Vel_DOF; ++i ) {
    currentCell[ i ] -= Inverse_Tau * ( currentCell[ i ] - feq[ i ]  );
  }

}

void doCollision( double *collideField,
                  int *flagField,
                  const double * const tau,
                  unsigned xlength ) {

	//Variable declaration
  //TODO : Ask Nicola if we need the pressure distribution

  double Density = 0.0;
  double Velocity[ Dimensions ];
  double Feq[ Vel_DOF ];
  unsigned TotalLength = xlength + 2;

  unsigned Current_Cell = 0;

	//Looping through individual element
	for( unsigned z = 1; z <= xlength; ++z )  {
		for( unsigned y = 1; y <= xlength; ++y )  {
			for( unsigned x = 1; x <= xlength; ++x ) {

				Current_Cell = computeFieldIndex( x, y, z, TotalLength );

				computeDensity( ( collideField + Current_Cell ) , &Density );
				computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
				computeFeq( &Density , Velocity , Feq );
				computePostCollisionDistributions( ( collideField + Current_Cell ) , tau , Feq );

			}
		}
	}
}
