#include "DataStructure.h"

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */

void scanBoundary( std::list<BoundaryFluid*>& ObstacleList,
					std::vector<Fluid*>& FluidDomain,
					std::vector<Fluid*>& VTKrepresentation,
                    int* flagField,
					int *VtkID,
					std::vector<BoundaryEntry*> BoundaryConditions );


void treatBoundary( double *collideField,
                    std::list<BoundaryFluid*>& BoundaryLayerList );

#endif
