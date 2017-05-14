#include "helper.h"
#include "boundary_val.h"

void boundaryvalues(int imax,//Grid size in x-direction
					int jmax,//Grid size in y-direction
					double **U,//2D array which stores U
					double **V)//2D array which stores V
{
	int i, j;
	
	
	for(i = 1; i <= imax; ++i) {
		//No-slip boundary conditions
		V[i][0] = 0;
		V[i][jmax] = 0;
		U[i][0] = -1.0 * U[i][1];
		//Following boundary condition which includes lid velocity
		U[i][jmax+1] = 2.0 - U[i][jmax]; 
	}
	
	for(j = 1; j <= jmax; ++j) {
		//No-slip boundary conditions
		U[0][j] = 0;
		U[imax][j] = 0;
		V[0][j] = -1.0 * V[1][j];
		V[imax+1][j] = -1.0 * V[imax][j]; 
	}
}						