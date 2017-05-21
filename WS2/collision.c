#include "collision.h"
#include "LBDefinitions.h"

void computePostCollisionDistributions(double *currentCell, const double * const tau, const double *const feq){
  /* TODO */
  double Inverse_Tau = 1.0 / tau;
  for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
    currentCell[ Vel_Component ] -= Inverse_Tau * ( currentCell[ Vel_Component ] - feq );
  }
}

void doCollision(double *collideField, int *flagField, const double * const tau, int xlength){
	/* TODO */
	//Variable declaration
  //TODO : Ask Nicola if we need the pressure distribution
	double Density = 0.0;
	double Velocity[ Dimensions ] = { 0.0 };
	double Feq[ Cell_Vel_DOF ] = { 0.0 };
	int X_Coordinate = 0, Y_Coordinate = 0, Z_Coordinate = 0;
  int Vel_Component = 0, Current_Cell = 0;

	//Looping through individual element
	for( Z_Coordinate = 1 ; Z_Coordinate < xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate < xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate < xlength ; ++X_Coordinate ) {

				Current_Cell =  Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate );

				computeDensity( (collideField + Current_Cell) , &Density );
				computeVelocity( (collideField + Current_Cell) , &Density , Velocity );
				computeFeq( &Density , Velocity , Feq );
				computePostCollisionDistributions( (collideField + Current_Cell) , tau , Feq );

			}
		}
	}
}
