#include "initLB.h"
#include "LBDefinitions.h"

int readParameters(int *xlength, double *tau, double *velocityWall, int *timesteps, int *timestepsPerPlotting, int argc, char *argv[]){
  /* TODO */



  return 0;
}


void initialiseFields(double *collideField, double *streamField, int *flagField, int xlength){
	/* TODO
  1. masking
  */

	//Variable declaration
	int X_Coordinate = 0 , Y_Coordinate = 0 , Z_Coordinate = 0;
	double Current_Cell = 0.0;

	int Square_xlength = xlength * xlength;

  //precomputed weight values
	double Initialization_Value_0 = 12.0 / 36.0 ;
	double Initialization_Value_1 =  2.0 / 36.0 ;
	double Initialization_Value_2 =  1.0 / 36.0 ;


	//all the fields are separately initialised for cache optimisation
	//Initialization of collideField loop unrolled
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
				Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
				collideField[ Current_Cell ]      = Initialization_Value_2 ;
				collideField[ Current_Cell + 1 ]  = Initialization_Value_2 ;
				collideField[ Current_Cell + 2 ]  = Initialization_Value_1 ;
				collideField[ Current_Cell + 3 ]  = Initialization_Value_2 ;
				collideField[ Current_Cell + 4 ]  = Initialization_Value_2 ;
				collideField[ Current_Cell + 5 ]  = Initialization_Value_2 ;
				collideField[ Current_Cell + 6 ]  = Initialization_Value_1 ;
				collideField[ Current_Cell + 7 ]  = Initialization_Value_2 ;
				collideField[ Current_Cell + 8 ]  = Initialization_Value_1 ;
				collideField[ Current_Cell + 9 ]  = Initialization_Value_0 ;
				collideField[ Current_Cell + 10 ] = Initialization_Value_1 ;
				collideField[ Current_Cell + 11 ] = Initialization_Value_2 ;
				collideField[ Current_Cell + 12 ] = Initialization_Value_1 ;
				collideField[ Current_Cell + 13 ] = Initialization_Value_2 ;
				collideField[ Current_Cell + 14 ] = Initialization_Value_2 ;
				collideField[ Current_Cell + 15 ] = Initialization_Value_2 ;
				collideField[ Current_Cell + 16 ] = Initialization_Value_1 ;
				collideField[ Current_Cell + 17 ] = Initialization_Value_2 ;
				collideField[ Current_Cell + 18 ] = Initialization_Value_2 ;
			}
		}
	}


	//Initialization of streamField loop unrolled
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
        Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
				streamField[ Current_Cell ]      = Initialization_Value_2 ;
				streamField[ Current_Cell + 1 ]  = Initialization_Value_2 ;
				streamField[ Current_Cell + 2 ]  = Initialization_Value_1 ;
				streamField[ Current_Cell + 3 ]  = Initialization_Value_2 ;
				streamField[ Current_Cell + 4 ]  = Initialization_Value_2 ;
				streamField[ Current_Cell + 5 ]  = Initialization_Value_2 ;
				streamField[ Current_Cell + 6 ]  = Initialization_Value_1 ;
				streamField[ Current_Cell + 7 ]  = Initialization_Value_2 ;
				streamField[ Current_Cell + 8 ]  = Initialization_Value_1 ;
				streamField[ Current_Cell + 9 ]  = Initialization_Value_0 ;
				streamField[ Current_Cell + 10 ] = Initialization_Value_1 ;
				streamField[ Current_Cell + 11 ] = Initialization_Value_2 ;
				streamField[ Current_Cell + 12 ] = Initialization_Value_1 ;
				streamField[ Current_Cell + 13 ] = Initialization_Value_2 ;
				streamField[ Current_Cell + 14 ] = Initialization_Value_2 ;
				streamField[ Current_Cell + 15 ] = Initialization_Value_2 ;
				streamField[ Current_Cell + 16 ] = Initialization_Value_1 ;
				streamField[ Current_Cell + 17 ] = Initialization_Value_2 ;
				streamField[ Current_Cell + 18 ] = Initialization_Value_2 ;
			}
		}
	}


	//Initialization of flagField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
        Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

        //TODO : masking
				if( Z_Coordinate == xlength )
					flagField[Current_Cell] = 2 ;
				else if(   X_Coordinate == 0 || X_Coordinate == xlength
						|| Y_Coordinate == 0 || Y_Coordinate == xlength )
					flagField[Current_Cell] = 1 ;
				else
					flagField[Current_Cell] = 0 ;
			}
		}
	}
}
