#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"



/* initialises the particle distribution functions and the flagfield */


void initialiseFields_LidDrivenCavity( double **collideField,
                                       double **streamField,
                                       int **flagField,
                                       int **IdField,
                                       unsigned* Length,
									   unsigned rank,
									   unsigned iProj,
									   unsigned jProj,
									   unsigned kProj ) ;


#endif
