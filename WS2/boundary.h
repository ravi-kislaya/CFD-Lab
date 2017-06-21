#include "DataStructure.h"

#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */
<<<<<<< HEAD
//void treatBoundary(double *collideField, int* flagField, const double * const wallVelocity,int xlength);
void treatBoundary( double *collideField,
                    int* flagField,
                    double * wallVelocity,
                    int xlength,
										int **NoSlip,
										int **MovingWall,
										double *MovingWallDotProduct );
void teachBoundary( int* flagField,
										int xlength,
										int **NoSlip,
										int **MovingWall,
										double *MovingWallDotProduct,
										double * wallVelocity);
double dotProduct(double * A , double *B);
=======

void scanBoundary( std::list<Fluid*>& ObstacleList,
                    int* flagField,
                    unsigned xlength,
                    double* wallVelocity );


void treatBoundary( double *collideField,
                    std::list<Fluid*>& BoundaryLayerList,
                    const double * const wallVelocity,
                    unsigned xlength );

>>>>>>> 0b2de49ba286ff6eee309f79c6ccf20705f2a343
#endif
