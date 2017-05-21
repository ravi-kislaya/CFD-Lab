#include "collision.h"

void computePostCollisionDistributions(double *currentCell, const double * const tau, const double *const feq){
  /* TODO */
}

void doCollision(double *collideField, int *flagField,const double * const tau,int xlength){
	/* TODO */
	//Variable declaration
	const int Cell_Vel_DOF = 19 ;
	const int Dimensions = 3 ;
	double Density;
	double Velocity[Dimensions];
	double Feq[Cell_Vel_DOF];
	int X_Coordinate , Y_Coordinate , Z_Coordinate , Vel_Component , Current_Cell ;
	
	//Looping through individual element				
	for( Z_Coordinate = 1 ; Z_Coordinate < xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate < xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate < xlength ; ++X_Coordinate ) {
				Current_Cell =  ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ;
				if( flagField[Current_Cell] == 0 ) { 
					computeDensity( Current_Cell , &Density );
					computeVelocity( Current_Cell , &Density , Velocity );
					computeFeq( &Density , Velocity , Feq );
					computePostCollisionDistributions( Current_Cell , tau , Feq );
				}
			}
		}
	}
}

