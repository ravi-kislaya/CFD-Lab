#include "streaming.h"

void doStreaming(double *collideField, double *streamField,int *flagField,int xlength){
	/* TODO */
	const int Cell_Vel_DOF = 19 ;
	int CX[Cell_Vel_DOF] = { 0 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , -1 ,
						0 , 1 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , 0 } ;
	int CY[Cell_Vel_DOF] = { -1 , 0 , 0 , 0 , 1 , -1 , -1 , -1 , 0 ,
						0 , 0 , 1 , 1 , 1 , -1 , 0 , 0 , 0 , 1 } ;
	int CZ[Cell_Vel_DOF] = { -1 , -1 , -1 , -1 , -1 , 0 , 0 , 0 , 0 ,
						 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 } ;
	
	int X_Coordinate , Y_Coordinate , Z_Coordinate , Vel_Component , Current_Cell , Target_Cell ;
	
	//Looping through individual element				
	for( Z_Coordinate = 1 ; Z_Coordinate < xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate < xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate < xlength ; ++X_Coordinate ) {
				Current_Cell =  ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ;
				if( flagField[Current_Cell] == 0 ) { 
					for( Vel_Component = 0 ; Vel_Component < Cell_Vel_DOF ; ++Vel_Component ){
						Target_Cell =   ( ( Z_Coordinate - CZ[Vel_Component] ) * Square_xlength )
									  + ( ( Y_Coordinate - CY[Vel_Component] )* xlength ) 
									  + ( X_Coordinate - CX[Vel_Component] ) ;
						streamField[Target_Cell] = collideField[Current_Cell] ;
					}
				}
			}
		}
	}
}

