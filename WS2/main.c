#ifndef _MAIN_C_
#define _MAIN_C_

#include "LBDefinitions.h"
#include "collision.h"
#include "streaming.h"
#include "initLB.h"
#include "visualLB.h"
#include "boundary.h"

// DEBUGGING
#include <stdio.h>

int main(int argc, char *argv[]){
  /* TODO */
  int xlength = 5;
  int TotalLength = xlength + 2;
  int CellNumber = TotalLength * TotalLength * TotalLength;
  double *collideField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  double *streamField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  int *flagField = ( int* )calloc( CellNumber, sizeof(int) );
  double wallVelocity[ 3 ] = { 1.0 ,1.0, 0.0 };
  int Square_xlength = xlength * xlength;

  int CountNoSlip = 0 ;
  int CountMovingWall = 0 ;
  int Current_Cell = 0;
  for( int Z_Coordinate = 1 ; Z_Coordinate <= xlength ; ++Z_Coordinate )  {
		for( int Y_Coordinate = 1 ; Y_Coordinate <= xlength ; ++Y_Coordinate )  {
			for( int X_Coordinate = 1 ; X_Coordinate <= xlength ; ++X_Coordinate ) {
        Current_Cell = ( ( Z_Coordinate * Square_xlength )
                      + ( Y_Coordinate * xlength ) + X_Coordinate ) ;
        if(flagField[Current_Cell] == WALL)
          ++CountNoSlip;
        if(flagField[Current_Cell] == MOVING_WALL)
          ++CountMovingWall;
      }
    }
  }
  int NoSlip[CountNoSlip][2] ;
  int MovingWall[CountMovingWall][2] ;
  double MovingWallDotProduct[CountMovingWall] ;

  initialiseFields( collideField,
                  streamField,
                  flagField,
                  xlength );

  teachBoundary(  flagField,
                  xlength,
                 NoSlip,
                 MovingWall,
                 MovingWallDotProduct,
                 wallVelocity);


  free( collideField );
  free( streamField );
  free( flagField );
  printf( "Hello\n" );

  return 0;
}

#endif
