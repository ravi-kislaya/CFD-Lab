#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"



/* initialises the particle distribution functions and the flagfield */
void initialiseFields( double *collideField,
                       double *streamField,
                       int *flagField,
                       unsigned* Length );

#endif
