#include "DataStructure.h"
#include <list>

#ifndef _VISUALLB_H_
#define _VISUALLB_H_

/** writes the density and velocity field (derived from the distributions in collideField)
 *  to a file determined by 'filename' and timestep 't'. You can re-use parts of the code
 *  from visual.c (VTK output for Navier-Stokes solver) and modify it for 3D datasets.
 */

void write_vtkHeader( FILE* fp,
                      std::vector<Fluid*>& FluidDomain );

void write_vtkPointCoordinates( FILE* fp,
                                std::vector<Fluid*>& FluidDomain );

void write_vtkPointElements( FILE* fp,
                             std::vector<Fluid*>& VTKrepresentation,
                             int* VtkID );

void writeVtkOutput( const char * filename,
                     int RANK,
                     double* const collideField,
                     int* VtkID,
                     std::vector<Fluid*>& FluidDomain,
                     std::vector<Fluid*>& VTKrepresentation,
                     unsigned int TimeStep );

#endif
