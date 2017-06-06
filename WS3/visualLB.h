#ifndef _VISUALLB_H_
#define _VISUALLB_H_

/** writes the density and velocity field (derived from the distributions in collideField)
 *  to a file determined by 'filename' and timestep 't'. You can re-use parts of the code
 *  from visual.c (VTK output for Navier-Stokes solver) and modify it for 3D datasets.
 */

void write_vtkHeader( FILE* fp, unsigned* Length );

void write_vtkPointCoordinates( FILE* fp, unsigned* Length );

void writeVtkOutput( double * const collideField,
                     const char * filename,
                     unsigned int t,
                     unsigned* Length );

#endif
