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

				Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
				//Cell wise streaming considering the velocity component
				for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
					Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
									+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
								  + ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );

					streamField[ Target_Cell + Vel_Component ] = collideField[ Current_Cell + Vel_Component ] ;

				}

			}
		}
	}
}
