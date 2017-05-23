#include "boundary.h"
#include "LBDefinitions.h"

void treatBoundary( double *collideField,
                    int* flagField,
                    const double * const wallVelocity,
                    int xlength ){
  /* TODO */
    int Square_xlength = xlength * xlength;
    int Current_Cell = 0;
    int z = 0;


    for ( int i = 1; i < xlength; ++i ) {
        for ( int j = 1; j < xlength; ++j ) {

            // XY plane with Z = 0
            Current_Cell = Vel_DOF
                         * ( ( z * Square_xlength ) + ( i * xlength ) + j );




        }
    }

}
