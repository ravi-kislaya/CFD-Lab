#include "boundary.h"
#include "LBDefinitions.h"
#include <math.h>

double dotProduct(double *A , double *B) {
	return (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]) ;
}

void mixedBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength,
					int Square_xlength,
					double Inverse_Square_CS,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){
	
	double Density = 1.0 ;
	double Velocity[3] = {0.0};
	int Target_Cell = 0 ;
	computeDensity( (collideField+Current_Cell) , Density ) ;
	computeVelocity( (collideField+Current_Cell) , Density , Velocity) ;
	
	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 1 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + abs(9-Vel_Component)] ;
		}
		if( flagField[Target_Cell] == 2 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + abs(9-Vel_Component)]
													   + ( 2 * LATTICEWEIGHTS[Vel_Component] * Density 
													   * dotProduct(Velocity,wallVelocity) * Inverse_Square_CS );
		}
	}
}

void movingBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength,
					int Square_xlength,
					double Inverse_Square_CS,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){
	
	double Density = 1.0 ;
	double Velocity[3] = {0.0};
	int Target_Cell = 0 ;
	computeDensity( (collideField+Current_Cell) , Density ) ;
	computeVelocity( (collideField+Current_Cell) , Density , Velocity) ;
	
	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 2 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + abs(9-Vel_Component)]
													   + ( 2 * LATTICEWEIGHTS[Vel_Component] * Density 
													   * dotProduct(Velocity,wallVelocity) * Inverse_Square_CS );
		}
	}
}

void noSlipBoundary( double *collideField,
                    int* flagField,
                    int xlength,
					int Square_xlength,
					int X_Coordinate,
					int Y_Coordinate,
					int Z_Coordinate,
					int Current_Cell){
						
	int Target_Cell = 0 ;
	
	for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
		Target_Cell = Vel_DOF * ( ( ( Z_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 2 ] ) * Square_xlength )
					+ ( ( Y_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 1 ] ) * xlength )
					+ ( X_Coordinate + LATTICEVELOCITIES[ Vel_Component ][ 0 ] ) );
		if( flagField[Target_Cell] == 1 ) {
			collideField[Current_Cell + Vel_Component] += collideField[Target_Cell + abs(9-Vel_Component)] ;
		}
	}
}


void treatBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength ){
  /* TODO */
	int X_Coordinate = 0, Y_Coordinate = 0, Z_Coordinate = 1;
	int Current_Cell = 0, Target_Cell = 0;
	int Square_xlength = xlength * xlength;
	double Inverse_Square_CS = 1.0 / ( C_S * C_S );

	// XY plane with Z = 1
    for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }
	
	//XY plane with Z = xlength excluding edges
	Z_Coordinate = xlength - 1 ;
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			movingBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength, 
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }
	
	//XY plane with Z = xlength only edges
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; X_Coordinate+=xlength-1 ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			mixedBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength, 
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }
	for ( Y_Coordinate = 1; Y_Coordinate < xlength; Y_Coordinate+=xlength-1 ) {
        for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			mixedBoundary( collideField, flagField, wallVelocity, xlength, Square_xlength, 
							Inverse_Square_CS, X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
        }
    }
	
	//YZ plane
	for ( Z_Coordinate = 2; Z_Coordinate < xlength - 2; ++Z_Coordinate ) {
		//YZ plane with X_Coordinate = 1
		X_Coordinate = 1;
		for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
		
		//YZ plane with X_Coordinate = xlength-1
		X_Coordinate = xlength-1;
		for ( Y_Coordinate = 1; Y_Coordinate < xlength; ++Y_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
	}
	
	//XZ plane
	for ( Z_Coordinate = 2; Z_Coordinate < xlength - 2; ++Z_Coordinate ) {
		//XZ plane with Y_Coordinate = 1
		Y_Coordinate = 1;
		for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
		
		//YZ plane with X_Coordinate = xlength-1
		Y_Coordinate = xlength-1;
		for ( X_Coordinate = 1; X_Coordinate < xlength; ++X_Coordinate ) {
			Current_Cell = Vel_DOF * ( ( Z_Coordinate * Square_xlength )
						 + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
			noSlipBoundary( collideField, flagField, xlength, Square_xlength,
							X_Coordinate, Y_Coordinate, Z_Coordinate, Current_Cell );
		}
	}
}
