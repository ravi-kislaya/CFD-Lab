#include "DataStructure.h"

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */

void scanBoundary( std::list<BoundaryFluid*>& ObstacleList,
					std::list<Fluid*>& FluidDomain,
					std::list<Fluid*>& VTKrepresentation,
                    int* flagField,
					int *IdField,
                    unsigned* Length,
                    double* wallVelocity,
					double* InletVel,
					double DeltaDensity );


void treatBoundary( double *collideField,
                    std::list<BoundaryFluid*>& BoundaryLayerList,
                    const double * const wallVelocity );

#endif
