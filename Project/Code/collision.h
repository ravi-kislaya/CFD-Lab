#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "computeCellValues.h"
#include <vector>
#include "DataStructure.h"

/** carries out the whole local collision process. Computes density and velocity and
 *  equilibrium distributions. Carries out BGK update.
 */
void doCollision( std::vector<Fluid*>& FluidDomain,
                  double *collideField,
                  const double * const tau );
#endif
