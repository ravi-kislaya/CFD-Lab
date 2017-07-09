#include <vector>
#include <list>

#include "helper.h"
#include "DataStructure.h"

#ifndef _INITLB_H_
#define _INITLB_H_




void initialiseData( double** collideField,
                     double** streamField,
					 int** flagField,
                     int** CpuID,
                     int** VtkID,
                     std::vector<Fluid*> &FluidDomain,
                     std::vector<BoundaryEntry*> &BoundaryElementVector,
                     std::unordered_map<unsigned, unsigned>& LocalToGlobalIdTable,
                     std::unordered_map<unsigned, unsigned>& GlobalToLocalIdTable,
                     int RANK );

#endif
