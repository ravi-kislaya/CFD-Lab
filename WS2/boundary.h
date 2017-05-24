#include "DataStructure.h"

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    int xlength,
                    double* wallVelocity );


void treatBoundary( double *collideField,
                    std::list<Fluid*>& BoundaryLayerList,
                    const double * const wallVelocity,
                    int xlength );

#endif
