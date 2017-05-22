#include "streaming.h"
#include "LBDefinitions.h"

void doStreaming(double *collideField, double *streamField,int *flagField,int xlength){
	/* TODO */


	int X_Coordinate = 0, Y_Coordinate = 0, Z_Coordinate = 0;
	int Vel_Component = 0, Current_Cell = 0, Target_Cell = 0;
	int Square_xlength = xlength * xlength;


	//Looping through individual element
	for( Z_Coordinate = 1 ; Z_Coordinate < xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate < xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate < xlength ; ++X_Coordinate ) {

				Current_Cell = Cell_Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;


				for( Vel_Component = 0 ; Vel_Component < Cell_Vel_DOF ; ++Vel_Component ) {

					Target_Cell = Cell_Vel_DOF * ( ( ( Z_Coordinate + CZ[ Vel_Component ] ) * Square_xlength )
									+ ( ( Y_Coordinate + CY[ Vel_Component ] ) * xlength )
								  + ( X_Coordinate + CX[ Vel_Component ] ) );

					streamField[ Target_Cell + Vel_Component ] = collideField[ Current_Cell + Vel_Component ] ;

				}

			}
		}
	}
}
