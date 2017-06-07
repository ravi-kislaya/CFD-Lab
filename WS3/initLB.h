#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"



/* initialises the particle distribution functions and the flagfield */
void initialiseFieldsStep( double *collideField,
						   double *streamField,
						   int *flagField,
						   int *IdField,
                           unsigned* Length );
						   
void initialiseFieldsChannel( double *collideField,
							  double *streamField,
							  int *flagField,
						      int *IdField,
                              unsigned* Length );


void initialiseFields_LidDtivenCavity( double *collideField,
                                       double *streamField,
                                       int *flagField,
                                       int *IdField,
                                       unsigned* Length );

#endif
