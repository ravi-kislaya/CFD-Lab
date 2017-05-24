#include "streaming.h"
#include "LBDefinitions.h"
#include "helper.h"

void doStreaming(double *collideField, double *streamField,int *flagField,int xlength){
	/* TODO */


	int Vel_Component = 0, Current_Cell = 0, Target_Cell = 0;


	//Looping through individual element
	for( int z = 1 ; z < xlength ; ++z )  {
		for( int y = 1 ; y < xlength ; ++y )  {
			for( int x = 1 ; x < xlength ; ++x ) {

				Current_Cell = computeFieldIndex( x, y, z, xlength );

				//Cell wise streaming considering the velocity component
				for( Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {


					Target_Cell = computeFieldIndex( x + LATTICEVELOCITIES[ Vel_Component ][ 0 ],
						 							 y + LATTICEVELOCITIES[ Vel_Component ][ 1 ],
													 z + LATTICEVELOCITIES[ Vel_Component ][ 2 ],
													 xlength );


					streamField[ Target_Cell + Vel_Component ]
							= collideField[ Current_Cell + Vel_Component ] ;
				}

			}
		}
	}
}
