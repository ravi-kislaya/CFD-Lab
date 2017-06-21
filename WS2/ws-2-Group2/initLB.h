#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"


/* reads the parameters for the lid driven cavity scenario from a config file */
int read_parameters( const char* INPUT_FILE_NAME,        /* the name of the data file */
                     unsigned* xlength,                       /* number of cells along x direction */
                     double* tau,                        /* relaxation time */
                     double* U,                          /* lid velocity x-direction */
                     double* V,                          /* lid velocity y-direction */
                     double* W,                          /* lid velocity z-direction */
                     unsigned* timesteps,                     /* number of simulation time steps */
                     unsigned* timestepsPerPlotting );        /* number of visualization time steps */



/* initialises the particle distribution functions and the flagfield */
void initialiseFields( double *collideField,
                       double *streamField,
                       int *flagField,
                       unsigned xlength);

#endif
