#ifndef _INITLB_H_
#define _INITLB_H_
#include "helper.h"



/* initialises the particle distribution functions and the flagfield */
void initialiseFieldsStep( double *collideField,
													 double *streamField,
						   						 int *flagField,
						   				 		 int *IdField,
                           unsigned* Length );

void initialiseFields_PlaneShearFlow( double *collideField,
							  											double *streamField,
							  											int *flagField,
						      										int *IdField,
                              				unsigned* Length );


void initialiseFields_LidDrivenCavity( double *collideField,
                                       double *streamField,
                                       int *flagField,
                                       int *IdField,
                                       unsigned* Length );

void initialiseFields_TiltedPlate( const char *PLATE_TXT_FILE_NAME
																	 double *collideField,
																	 double *streamField,
																	 int *flagField,
																	 int *IdField,
																	 unsigned* Length );

																			 

#endif
