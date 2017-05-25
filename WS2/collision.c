#include "collision.h"
#include "LBDefinitions.h"
#include "helper.h"


void computePostCollisionDistributions( double *currentCell,
                                        const double * const tau,
                                        const double *const feq ) {
  /* TODO */
  double Inverse_Tau = 1.0 / ( *tau );
  for( int i = 0; i < Vel_DOF; ++i ) {
	//printf("%f  \n",currentCell[ i ] - (Inverse_Tau * ( currentCell[ i ] - ( *feq )) ));
    currentCell[ i ] -= Inverse_Tau * ( currentCell[ i ] - feq[ i ]  );
	
  }
}

void doCollision( double *collideField,
                  int *flagField,
                  const double * const tau,
                  int xlength ) {
	/* TODO */
	//Variable declaration
  //TODO : Ask Nicola if we need the pressure distribution

  double Density = 0.0;
  double Velocity[ Dimensions ];
  double Feq[ Vel_DOF ];

  int Current_Cell = 0;

	//Looping through individual element
	for( int z = 1; z <= xlength; ++z )  {
		for( int y = 1; y <= xlength; ++y )  {
			for( int x = 1; x <= xlength; ++x ) {

				Current_Cell = computeFieldIndex( x, y, z, xlength );

				computeDensity( ( collideField + Current_Cell ) , &Density );
				computeVelocity( ( collideField + Current_Cell ) , &Density , Velocity );
				computeFeq( &Density , Velocity , Feq );
				computePostCollisionDistributions( (collideField + Current_Cell) , tau , Feq );

			}
		}
	}
}
