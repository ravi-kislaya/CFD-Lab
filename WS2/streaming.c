#include "streaming.h"
#include "LBDefinitions.h"
#include "helper.h"

void doStreaming( double *collideField,
	 			  double *streamField,
				  int *flagField,
				  unsigned xlength ) {


	unsigned Fluid_Cell = 0;
	unsigned Neighbour_Cell = 0;
	unsigned TotalLength = xlength + 2;

<<<<<<< HEAD
	//Looping through individual element
	for( Z_Coordinate = 1 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( Y_Coordinate = 1 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( X_Coordinate = 1 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
=======
>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343

	//Looping through all fluid element
	for( unsigned z = 1 ; z <= xlength ; ++z )  {
		for( unsigned y = 1 ; y <= xlength ; ++y )  {
			for( unsigned x = 1 ; x <= xlength ; ++x ) {

				Fluid_Cell = computeFieldIndex( x, y, z, TotalLength );

				//Cell wise streaming considering the velocity component
				for( unsigned Vel_Component = 0; Vel_Component < Vel_DOF; ++Vel_Component ) {


					Neighbour_Cell = computeFieldIndex( x + LATTICEVELOCITIES[ Vel_Component ][ 0 ],
						 							 	y + LATTICEVELOCITIES[ Vel_Component ][ 1 ],
													 	z + LATTICEVELOCITIES[ Vel_Component ][ 2 ],
													 	TotalLength );


					streamField[ Fluid_Cell + 18 - Vel_Component ]
							= collideField[ Neighbour_Cell + 18 - Vel_Component ] ;
				}
			}
		}
	}
}
