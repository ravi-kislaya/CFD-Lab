#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

/** handles the boundaries in our simulation setup */
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
#endif
