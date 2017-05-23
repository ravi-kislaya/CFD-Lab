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
	int X_Coordinate = 0 , Y_Coordinate = 0 , Z_Coordinate = 0 , Vel_Component = 0;
	int Current_Cell = 0.0;

	int Square_xlength = xlength * xlength;


	//all the fields are separately initialised for cache optimisation

  //Initialization of collideField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {

				Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

        for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
          collideField [Current_Cell + Vel_Component] = LATTICEWEIGHTS[Vel_Component] ;
        }

			}
		}
	}

  //Initialization of streamField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
        Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

        for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
          streamField [Current_Cell + Vel_Component] = LATTICEWEIGHTS[Vel_Component] ;
        }

			}
		}
	}


	//Initialization of flagField
	for( Z_Coordinate = 0 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 0 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 0 ; X_Coordinate <= xlength ; ++X_Coordinate ) {

        Current_Cell = ( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;

        //TODO : masking
				if( Z_Coordinate == xlength )
					flagField [Current_Cell] = 2 ;
				else if(   X_Coordinate == 0 || X_Coordinate == xlength
						|| Y_Coordinate == 0 || Y_Coordinate == xlength )
					flagField [Current_Cell] = 1 ;
				else
					flagField [Current_Cell] = 0 ;
			}
		}
	}


}
