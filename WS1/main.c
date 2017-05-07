#include "helper.h"
#include "visual.h"
#include "init.h"
#include "uvp.h"
#include "boundary_val.h"
#include "sor.h"
#include <stdio.h>


/**
 * The main operation reads the configuration file, initializes the scenario and
 * contains the main loop. So here are the individual steps of the algorithm:
 *
 * - read the program configuration file using read_parameters()
 * - set up the matrices (arrays) needed using the matrix() command
 * - create the initial setup init_uvp(), init_flag(), output_uvp()
 * - perform the main loop
 * - trailer: destroy memory allocated and do some statistics
 *
 * The layout of the grid is decribed by the first figure below, the enumeration
 * of the whole grid is given by the second figure. All the unknowns corresond
 * to a two dimensional degree of freedom layout, so they are not stored in
 * arrays, but in a matrix.
 *
 * @image html grid.jpg
 *
 * @image html whole-grid.jpg
 *
 * Within the main loop the following big steps are done (for some of the 
 * operations a definition is defined already within uvp.h):
 *
 * - calculate_dt() Determine the maximal time step size.
 * - boundaryvalues() Set the boundary values for the next time step.
 * - calculate_fg() Determine the values of F and G (diffusion and confection).
 *   This is the right hand side of the pressure equation and used later on for
 *   the time step transition.
 * - calculate_rs()
 * - Iterate the pressure poisson equation until the residual becomes smaller
 *   than eps or the maximal number of iterations is performed. Within the
 *   iteration loop the operation sor() is used.
 * - calculate_uv() Calculate the velocity at the next time step.
 */
int main(int argn, char** args){

	// declaring the matrices
	double **U, **V, **P;
	
	double **RS, **F, **G;

	// Declaring variables for the parameters that will be read from the input file
	double Re, UI, VI, PI, GX, GY, t_end, xlength, ylength, dt, dx, dy, alpha, omg, tau, eps, dt_value;
	
	int  imax, jmax, itermax;
	
	/* Reading parameters from input file */
	read_parameters("cavity100.dat", &Re, &UI, &VI, &PI, &GX, &GY, &t_end,&xlength, &ylength, &dt, &dx, &dy, &imax, &jmax, &alpha, &omg, &tau, &itermax, &eps, &dt_value);

	// Allocating memory for matrices

	//DELETE COMMENT: This function is in helper.c
	U = matrix(0, imax + 1, 0, jmax + 1);
	V = matrix(0, imax + 1, 0, jmax + 1);
	P = matrix(0, imax + 1, 0, jmax + 1);

	RS = matrix(0, imax + 1, 0, jmax + 1);
	F = matrix(0, imax, 0, jmax + 1);
	G = matrix(0, imax + 1, 0, jmax);

	// Initialization
	init_uvp(UI, VI, PI, imax, jmax, U, V, P);
	
	//Some additional variables
	double t = 0.0;
	double res = 0.0;
	int n = 0;
	int iternum = 0;

	//The main while loop that iterates over time
	while (t < t_end) {
		
		//DELETE Comment Can tau be = 0?? or only > 0
		//Calcaulte the time step
		if (tau >= 0.0) calculate_dt(Re, tau, &dt, dx, dy, imax, jmax, U, V);
		
		// Set the boundary for U and V
		boundaryvalues(imax, jmax, U, V);

		// Calcualte F and G
		calculate_fg(Re, GX, GY, alpha, dt, dx, dy, imax, jmax, U, V, F, G);
		
		// Calculate the RHS of Pressure Poisson Equation
		calculate_rs(dt, dx, dy, imax, jmax, F, G, RS);

		iternum = 0;
		res = 0.0;
		
		//Perform SOR
		while (iternum < itermax && res > eps) {
			sor (omg, dx, dy, imax, jmax, P, RS, &res); //Perform one SOR iteration
			iternum++;
		}

		// Update the velocities U and V
		calculate_uv(dt, dx, dy, imax, jmax, U, V, F, G, P);
		
		//DELETE Comment We can also write the vtk at the end
		//Output vtk file every 50th timestep
		if (n % 50 == 0) write_vtkFile("Cavity100", n, xlength, ylength, imax, jmax, dx, dy, U, V, P);
		
		//Update the loop variables
		t += dt;
		++n;
	}
	
	//Free the memory held by matrix
	free_matrix(U, 0, imax + 1, 0, jmax + 1);
	free_matrix(V, 0, imax + 1, 0, jmax + 1);
	free_matrix(P, 0, imax + 1, 0, jmax + 1);

	free_matrix(RS, 0, imax + 1, 0, jmax + 1);
	free_matrix(F, 0, imax, 0, jmax + 1);
	free_matrix(G, 0, imax + 1, 0, jmax);

	return -1;
}
