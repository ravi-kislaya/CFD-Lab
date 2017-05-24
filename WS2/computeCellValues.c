#include "computeCellValues.h"
#include "LBDefinitions.h"

void computeDensity( const double *const currentCell, double *density ){

  *density = 0.0;

  for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
    *density += currentCell[ Vel_Component ];
  }

}

void computeVelocity(const double * const currentCell, const double * const density, double *velocity){

  velocity[ 0 ] = 0.0;
  velocity[ 1 ] = 0.0;
  velocity[ 2 ] = 0.0;
  double Inverse_Density = 1.0 / ( *density );

  for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {
    velocity[ 0 ] += currentCell[ Vel_Component ]  * LATTICEVELOCITIES[ Vel_Component ][ 0 ];
    velocity[ 1 ] += currentCell[ Vel_Component ]  * LATTICEVELOCITIES[ Vel_Component ][ 1 ];
    velocity[ 2 ] += currentCell[ Vel_Component ]  * LATTICEVELOCITIES[ Vel_Component ][ 2 ];
  }

  velocity[ 0 ] *= Inverse_Density;
  velocity[ 1 ] *= Inverse_Density;
  velocity[ 2 ] *= Inverse_Density;
}


void computeFeq(const double * const density, const double * const velocity, double *feq){

 //CS means Speed of Sound
  double temp1 = 0.0, temp2 = 0.0;

 for( int Vel_Component = 0 ; Vel_Component < Vel_DOF ; ++Vel_Component ) {

   temp1 = 0.0;
   temp2 = 0.0;

   for( int i = 0 ; i < Dimensions ; ++i ) {
     //dot product of c and U
     temp1 += LATTICEVELOCITIES[ Vel_Component ][ i ] * velocity[ i ];

     //dot product of U and U
     temp2 += velocity[ i ] * velocity[ i ];
   }

   feq[ Vel_Component ] = LATTICEWEIGHTS[ Vel_Component ] * ( *density )
                        * ( 1 + ( InverseCS_Square * temp1 )
                            + ( 0.5 * temp1 * temp1 * InverseCS_Four )
                            - ( temp2 * InverseCS_Square * 0.5 ) );
 }

}
