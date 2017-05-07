#include "helper.h"
#include "uvp.h"

/**
*This function choses the time step size
*to avoid instability
*/
void calculate_dt(  double Re,//Reynolds Number
					double tau,//safety factor
					double *dt,//Time Step
					double dx,//Grid size in x-direction
					double dy,//Grid size in y-direction
					int imax,//number of cells x-direction
					int jmax,//number of cells y-direction
					double **U,//2D array which stores U
					double **V)//2D array which stores V
{
	double dt_Reynolds = 0.5 * Re / (( 1.0 / ( dx * dx )) + (1.0 / (dy * dy)));
	double dt_Umax = dx / absoluteMaxInArray( U, imax, jmax );
	double dt_Vmax = dy / absoluteMaxInArray( V, imax, jmax );
	*dt = tau * fmin( dt_Reynolds, fmin( dt_Umax, dt_Vmax ) );
}

void calculate_fg(  double Re,//Reynold's Number of the flow
					double GX,//acceleration due to gravity in x-direction
					double GY,//acceleration due to gravity in y-direction
					double alpha,//parameter
					double dt,//time step
					double dx,//Step size in x-direction
					double dy,//Step size in y-direction
					int imax,//number of cells x-direction
					int jmax,//number of cells y-direction
					double **U,
					double **V,
					double **F,
					double **G)
{
	int i,j;
	/*
	*To Do : write the names of variables and their meaning
	*/
	double d2udx2,d2udy2,du2dx,duvdy,d2vdx2,d2vdy2,duvdx,dv2dy;
	double inverse_dxSquare = 1.0 / ( dx * dx );
	double inverse_dySquare = 1.0 / ( dy * dy );
	double inverse_4dx = 1.0 / ( 4.0 * dx );
	double inverse_4dy = 1.0 / ( 4.0 * dy );
	double inverseRe = 1.0 / Re;

	for( i=1; i < imax; ++i )	{
		for( j=1; j <= jmax; ++j ) {
			
			d2udx2 = ( U[i+1][j] - 2.0 * U[i][j] + U[i-1][j] ) * inverse_dxSquare;
			
			
			d2udy2 = ( U[i][j+1] - 2.0 * U[i][j] + U[i][j-1] ) * inverse_dySquare;
			
			
			du2dx = ( ( ( U[i][j] + U[i+1][j] ) * ( U[i][j] + U[i+1][j] )  
						- ( U[i-1][j] + U[i][j] ) * ( U[i-1][j] + U[i][j] ) )
					  + alpha * ( fabs( U[i][j] + U[i+1][j] ) * ( U[i][j] - U[i+1][j] )
					            - fabs( U[i-1][j] + U[i][j] ) * ( U[i-1][j] - U[i][j] ) ) ) 
					* inverse_4dx;
					
					
			duvdy = ( ( ( V[i][j] + V[i+1][j] ) * ( U[i][j] + U[i][j+1] )  
						- ( V[i][j-1] + V[i+1][j-1] ) * ( U[i][j-1] + U[i][j] ) )
					  + alpha * ( fabs( V[i][j] + V[i+1][j] ) * ( U[i][j] - U[i][j+1] )
					            - fabs( V[i][j-1] + V[i+1][j-1] ) * ( U[i][j-1] - U[i][j] ) ) ) 
					* inverse_4dy;
					
					
			F[i][j] = U[i][j] + dt * ( ( ( d2udx2 + d2udy2 ) * inverseRe ) - du2dx - duvdy + GX );
			
		}
	}
	
	
	for( i=1; i <= imax; ++i )	{
		for( j=1; j < jmax; ++j )		{
			
			d2vdx2 = ( V[i+1][j] - 2.0 * V[i][j] + V[i-1][j] ) * inverse_dxSquare;
			
			
			d2vdy2 = ( V[i][j+1] - 2.0 * V[i][j] + V[i][j-1] ) * inverse_dySquare;
			
			
			dv2dy = ( ( ( V[i][j] + V[i][j+1] ) * ( V[i][j] + V[i][j+1] )  
						- ( V[i][j-1] + V[i][j] ) * ( V[i][j-1] + V[i][j] ) )
					  + alpha * ( fabs( V[i][j] + V[i][j+1] ) * ( V[i][j] - V[i][j+1] )
					            - fabs( V[i][j-1] + V[i][j] ) * ( V[i][j-1] + V[i][j] ) ) ) 
					* inverse_4dy;
					
					
			duvdx = ( ( ( V[i][j] + V[i+1][j] ) * ( U[i][j] + U[i][j+1] )  
						- ( V[i-1][j] + V[i][j] ) * ( U[i-1][j] + U[i-1][j+1] ) )
					  + alpha * ( fabs( U[i][j] + U[i][j+1] ) * ( V[i][j] - V[i+1][j] )
					            - fabs( U[i-1][j] + U[i-1][j+1] ) * ( V[i-1][j] - V[i][j] ) ) ) 
					* inverse_4dx;
					
					
			G[i][j] = V[i][j] + dt * ( ( ( d2vdx2 + d2vdy2 ) * inverseRe ) - dv2dy - duvdx + GY);
		}
	}
	
	
	//for boundary values of F
	for( j = 1; j <= jmax; ++j )	{
		F[0][j] = U[0][j];
		F[imax][j] = U[imax][j];
	}
	
	
	//for boundary values of G
	for( i=1;i <= imax; ++i )	{
		G[i][0] = V[i][0];
		G[i][jmax] = V[i][jmax];
	}
}

void calculate_rs(  double dt,
					double dx,
					double dy,
					int imax,
					int jmax,
					double **F,
					double **G,
					double **RS)
{	
	double inverse_dt = 1.0 / dt;
	double inverse_dx = 1.0 / dx;
	double inverse_dy = 1.0 / dy;
	
	int i,j;	
	for( i = 1; i <= imax; ++i )	{
		for( j = 1; j <= jmax; ++j )	{
			RS[i][j] = ( ( ( F[i][j] - F[i-1][j] ) * inverse_dx ) + ( ( G[i][j] - G[i][j-1] ) * inverse_dy ) )
						* inverse_dt;
		}
	}

}

void calculate_uv(  double dt,
					double dx,
					double dy,
					int imax,
					int jmax,
					double **U,
					double **V,
					double **F,
					double **G,
					double **P)
{
	double inverse_dx = 1.0 / dx;
	double inverse_dy = 1.0 / dy;
	
	int i,j;
	for( i = 1; i < imax; ++i )	{
		for( j = 1; j <= imax; ++j )	{
			U[i][j] = F[i][j] - dt * inverse_dx * ( P[i+1][j] - P[i][j] );
		}
	}
	for( i = 1; i <= imax; ++i )	{
		for( j = 1; j < jmax; ++j )		{
			V[i][j] = G[i][j] - dt * inverse_dy * ( P[i][j+1] - P[i][j] );
		}
	}
}