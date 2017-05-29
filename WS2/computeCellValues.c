#include "computeCellValues.h"
#include "LBDefinitions.h"
#include<stdio.h>

//Solves Equation 10
void computeDensity( const double *const currentCell, double *density ) {

  *density = 0.0;
  for( int i = 0; i < Vel_DOF; ++i ) {
    *density += currentCell[ i ];
  }

}
//Solves Equation 10
void computeVelocity( const double * const currentCell,
                      const double * const density,
                      double *velocity) {

  velocity[ 0 ] = 0.0;
  velocity[ 1 ] = 0.0;
  velocity[ 2 ] = 0.0;
  double Inverse_Density = 1.0 / ( *density );


  for( int i = 0 ; i < Vel_DOF ; ++i ) {
    velocity[ 0 ] += currentCell[ i ] * LATTICEVELOCITIES[ i ][ 0 ];
    velocity[ 1 ] += currentCell[ i ] * LATTICEVELOCITIES[ i ][ 1 ];
    velocity[ 2 ] += currentCell[ i ] * LATTICEVELOCITIES[ i ][ 2 ];
  }


  velocity[ 0 ] *= Inverse_Density;
  velocity[ 1 ] *= Inverse_Density;
  velocity[ 2 ] *= Inverse_Density;
}


void computeFeq( const double * const density,
                 const double * const velocity,
                 double *feq ) {

 //CS is Speed of Sound
  double temp1 = 0.0, temp2 = 0.0;
// Solves equation 11
 for( int i = 0; i < Vel_DOF; ++i ) {

   // temp1 = ci * ui
   temp1 = LATTICEVELOCITIES[ i ][ 0 ] * velocity[ 0 ]
         + LATTICEVELOCITIES[ i ][ 1 ] * velocity[ 1 ]
         + LATTICEVELOCITIES[ i ][ 2 ] * velocity[ 2 ];
   // temp2 = u * u
   temp2 = velocity[ 0 ] * velocity[ 0 ]
         + velocity[ 1 ] * velocity[ 1 ]
         + velocity[ 2 ] * velocity[ 2 ];



   feq[ i ] = LATTICEWEIGHTS[ i ] * ( *density ) * ( 1 + ( InverseCS_Square * temp1 )
                                  + ( 0.5 * temp1 * temp1 * InverseCS_Four )
                                  - ( temp2 * InverseCS_Square * 0.5 ) );
 }

}
