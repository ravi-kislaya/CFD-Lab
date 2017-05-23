#include "initLB.h"
#include "LBDefinitions.h"

int readParameters(int *xlength, double *tau, double *velocityWall, int *timesteps, int *timestepsPerPlotting, int argc, char *argv[]){
  /* TODO */
  	//DELETE COMMENT: I don't know why argc is used here instead of being used in main.

	if ( argc == 2 ) {
		const char *szFilename = NULL;
		szFilename = argv[1];
		READ_INT( szFilename, *xlength );
		READ_DOUBLE( szFilename, *tau );
		read_double( szFileName, "velocityWall1", &velocityWall[0] );
		read_double( szFileName, "velocityWall2", &velocityWall[1] );
		read_double( szFileName, "velocityWall3", &velocityWall[2] );
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

					Current_Cell = Vel_DOF *( ( Z_Coordinate * Square_xlength )
										    + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
		
						//TODO : masking
					if( Z_Coordinate == xlength )
						for( Vel_Component = 0 ;Vel_Component <Vel_DOF ; ++Vel_Component ) 	
							flagField [Current_Cell + Vel_Component] = 2 ;
					else if(   X_Coordinate == 0 || X_Coordinate == xlength
							|| Y_Coordinate == 0 || Y_Coordinate == xlength )
						for( Vel_Component = 0 ;Vel_Component <Vel_DOF ; ++Vel_Component ) 	
							flagField [Current_Cell + Vel_Component] = 1 ;
					else
						for( Vel_Component = 0 ;Vel_Component <Vel_DOF ; ++Vel_Component ) 
							flagField [Current_Cell + Vel_Component] = 0 ;
					
			}
		}
	}


}
