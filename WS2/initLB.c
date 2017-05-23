#include "initLB.h"

int readParameters(int *xlength, double *tau, double *velocityWall, int *timesteps, int *timestepsPerPlotting, int argc, char *argv[]){
	//DELETE COMMENT: I don't know why argc is used here instead of being used in main.

	if ( argc == 2 ) {
		const char *szFilename = NULL;
		szFilename = argv[1];
		READ_INT( szFilename, *xlength );
		READ_DOUBLE( szFilename, *tau );
		READ_DOUBLE( szFilename, *velocityWall ); //TODO: I am not sure about this BC. I think we have to set the side walls to zero and top wall to 1.
		READ_INT( szFilename, *timesteps );
		READ_INT( szFilename , *timestepsPerPlotting );
	}

	else {
		printf("Error : The input is wrong. Pass an input file as a parameter. \n"); // DELETE: I couldn't think of a reasonable error message
		return -1;
	}

	return 0;
}


void initialiseFields(double *collideField, double *streamField, int *flagField, int xlength){
	/* TODO */
	//Variable declaration
	const int Cell_Vel_DOF = 19 ;
	const int Dimensions = 3 ;
	
	int X_Coordinate , Y_Coordinate , Z_Coordinate , Vel_Component ;
	int Absolute_Sum ;
	double Current_Cell ; 
	int Square_xlength = xlength * xlength ;

	int CX[Vel_DOF] = { 0 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , -1 ,
						0 , 1 , -1 , 0 , 1 , 0 , -1 , 0 , 1 , 0 } ;
	int CY[Vel_DOF] = { -1 , 0 , 0 , 0 , 1 , -1 , -1 , -1 , 0 ,
						0 , 0 , 1 , 1 , 1 , -1 , 0 , 0 , 0 , 1 } ;
	int CZ[Vel_DOF] = { -1 , -1 , -1 , -1 , -1 , 0 , 0 , 0 , 0 ,
						 0 , 0 , 0 , 0 , 0 , 1 , 1 , 1 , 1 , 1 } ;
	
	double Initialization_Value_0 = 12.0 / 36.0 ;
	double Initialization_Value_1 =  2.0 / 36.0 ;
	double Initialization_Value_2 =  1.0 / 36.0 ;
	double InitValue[Dimensions]  = { Initialization_Value_0 , Initialization_Value_1 , Initialization_Value_2 } ;
	
	
	//all the fields are separately initialised for cache optimisation
	//Initialization of collideField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
				Current_Cell =  ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ;
			/*	collideField[Current_Cell]      = Initialization_Value_2 ;
				collideField[Current_Cell + 1]  = Initialization_Value_2 ;
				collideField[Current_Cell + 2]  = Initialization_Value_1 ;
				collideField[Current_Cell + 3]  = Initialization_Value_2 ;
				collideField[Current_Cell + 4]  = Initialization_Value_2 ;
				collideField[Current_Cell + 5]  = Initialization_Value_2 ;
				collideField[Current_Cell + 6]  = Initialization_Value_1 ;
				collideField[Current_Cell + 7]  = Initialization_Value_2 ;
				collideField[Current_Cell + 8]  = Initialization_Value_1 ;
				collideField[Current_Cell + 9]  = Initialization_Value_0 ;
				collideField[Current_Cell + 10] = Initialization_Value_1 ;
				collideField[Current_Cell + 11] = Initialization_Value_2 ;
				collideField[Current_Cell + 12] = Initialization_Value_1 ;
				collideField[Current_Cell + 13] = Initialization_Value_2 ;
				collideField[Current_Cell + 14] = Initialization_Value_2 ;
				collideField[Current_Cell + 15] = Initialization_Value_2 ;
				collideField[Current_Cell + 16] = Initialization_Value_1 ;
				collideField[Current_Cell + 17] = Initialization_Value_2 ;
				collideField[Current_Cell + 18] = Initialization_Value_2 ;
			*/
				for( Vel_Component = 0 ; Vel_Component < Cell_Vel_DOF ; ++Vel_Component ){
					Absolute_Sum = abs( CX[Vel_Component] ) + abs( CY[Vel_Component] ) + abs( CZ[Vel_Component] ) ;
					collideField[Current_Cell + Vel_Component] = InitValue[Absolute_Sum] ;
				}
			}
		}
	}
	
	
	//Initialization of streamField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
				Current_Cell =  ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ;
				streamField[Current_Cell]      = Initialization_Value_2 ;
				streamField[Current_Cell + 1]  = Initialization_Value_2 ;
				streamField[Current_Cell + 2]  = Initialization_Value_1 ;
				streamField[Current_Cell + 3]  = Initialization_Value_2 ;
				streamField[Current_Cell + 4]  = Initialization_Value_2 ;
				streamField[Current_Cell + 5]  = Initialization_Value_2 ;
				streamField[Current_Cell + 6]  = Initialization_Value_1 ;
				streamField[Current_Cell + 7]  = Initialization_Value_2 ;
				streamField[Current_Cell + 8]  = Initialization_Value_1 ;
				streamField[Current_Cell + 9]  = Initialization_Value_0 ;
				streamField[Current_Cell + 10] = Initialization_Value_1 ;
				streamField[Current_Cell + 11] = Initialization_Value_2 ;
				streamField[Current_Cell + 12] = Initialization_Value_1 ;
				streamField[Current_Cell + 13] = Initialization_Value_2 ;
				streamField[Current_Cell + 14] = Initialization_Value_2 ;
				streamField[Current_Cell + 15] = Initialization_Value_2 ;
				streamField[Current_Cell + 16] = Initialization_Value_1 ;
				streamField[Current_Cell + 17] = Initialization_Value_2 ;
				streamField[Current_Cell + 18] = Initialization_Value_2 ;
			}
		}
	}
	
	
	//Initialization of flagField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
				Current_Cell =  ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ;
				if( Z_Coordinate == 0 )
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

