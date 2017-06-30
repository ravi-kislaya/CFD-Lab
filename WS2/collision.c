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
<<<<<<< HEAD
	for( Z_Coordinate = 1 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
=======
	for( unsigned z = 1; z <= xlength; ++z )  {
		for( unsigned y = 1; y <= xlength; ++y )  {
			for( unsigned x = 1; x <= xlength; ++x ) {
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

				Current_Cell = computeFieldIndex( x, y, z, TotalLength );

				computeDensity( ( collideField + Current_Cell ) , &Density );
				computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
				computeFeq( &Density , Velocity , Feq );
				computePostCollisionDistributions( ( collideField + Current_Cell ) , tau , Feq );

			}
		}
	}
}
