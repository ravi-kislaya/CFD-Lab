#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <stdio.h>


void computePostCollisionDistributions( double *currentCell,
                                        const double * const tau,
                                        const double *const feq ) {

  double Inverse_Tau = 1.0 / ( *tau );

  for( int i = 0; i < Vel_DOF; ++i ) {
    currentCell[ i ] -= Inverse_Tau * ( currentCell[ i ] - feq[ i ]  );
  }

}

void doCollision( double *collideField,
                  int *flagField,
                  const double * const tau,
                  int xlength ) {

	//Variable declaration
  //TODO : Ask Nicola if we need the pressure distribution

  double Density = 0.0;
  double Velocity[ Dimensions ];
  double Feq[ Vel_DOF ];
  int TotalLength = xlength + 2;

  int Current_Cell = 0;

	//Looping through individual element
	for( int z = 1; z <= xlength; ++z )  {
		for( int y = 1; y <= xlength; ++y )  {
			for( int x = 1; x <= xlength; ++x ) {

				Current_Cell = computeFieldIndex( x, y, z, TotalLength );

				computeDensity( ( collideField + Current_Cell ) , &Density );
				computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
				computeFeq( &Density , Velocity , Feq );
				computePostCollisionDistributions( ( collideField + Current_Cell ) , tau , Feq );

			}
		}
	}
}
