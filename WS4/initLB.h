#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"



/* initialises the particle distribution functions and the flagfield */


void initialiseFields_LidDrivenCavity( double **collideField,
                                       double **streamField,
                                       int **flagField,
                                       int **IdField,
                                       unsigned* Length,
									   unsigned RANK,
                                       int* PROC,
									   int* CpuCoordinates ) ;


#endif
