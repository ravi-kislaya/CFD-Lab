#include "initLB.h"
#include "LBDefinitions.h"
#include "helper.h"
#include <iostream>

//TODO: DO final optimization for variables in for loop

void initialiseFields( double *collideField,
					   double *streamField,
					   int *flagField,
					   int *xlength ) {


	//Variable declaration
	int Current_Field_Cell = 0;
	int Current_Flag_Cell = 0;
	unsigned half_xLength = xlength[0]/2;

	//Initialization of collideField
	//1.

	//2. Plane shear flow
	for (unsigned z = 1; z <= xlength[2]; ++z) {
		for (unsigned y = 1; y <= xlength[1]; ++y) {
			for (unsigned x = 1; x <= xlength[0]; ++x) {
				flagField[computeFlagIndex(x, y, z, xlength)] = FLUID;
			}
		}
	}
	/* Boundary condition on Z = 0 is set to PRESSURE_IN and Z = Zmax is set OUTFLOW */
	for (unsigned x = 0; x < xlength[0] + 2; ++x) {
		for (unsigned y = 0; y < xlength[1] + 2; ++y) {
			flagField[computeFieldIndex(x, y, 0, xlength)] = PRESSURE_IN; //Left face
			flagField[computeFieldIndex(x, y, xlength[2] + 1, xlength)] = OUTFLOW; //Right face
		}
	}
	/* Boundary condition on X = 0 and X = Xmax is set NO_SLIP */
	for (unsigned y = 0; y < xlength[1] + 2; ++y) {
		for (unsigned z = 0; z < xlength[2] + 2; ++z) {
			flagField[computeFieldIndex(0, y, z, xlength)] = FREE_SLIP; //bottom face
			flagField[computeFieldIndex(xlength[0] + 1, y, z, xlength)] = FREE_SLIP; //top face
		}
	}
	/* Boundary condition on Y = 0 and Y = Ymax is set NO_SLIP */
	for (int x = 0; x < xlength[0] + 2; ++x) {
		for (int z = 0; z < xlength[2] + 2; ++z) {
			flagField[computeFieldIndex(x, 0, z, xlength)] = NO_SLIP; //face towards the direction of the screen
			flagField[computeFieldIndex(x, xlength[1] + 1, z, xlength)] = NO_SLIP; //face away from the direction of the screen
		}
	}


	//3. Flow over a step
	for( unsigned z = 0 ; z < xlength[2] + 2; ++z ) {
		for (unsigned y = 0; y < xlength[2] + 2; ++y) {
			for (unsigned x = 0; x < xlength[0] + 2; ++x) {

				flagField[computeFlagIndex(x, y, z, xlength)] = FLUID;
			}
		}
	}

	/* Boundary condition on Z = 0 is set to INFLOW and Z = Zmax is set OUTFLOW */
	for ( unsigned y = 0; y < xlength[1] + 2; ++y) {
		for ( unsigned x = 0; x < xlength[0] + 2; ++x) {
			flagField[computeFieldIndex(x, y, 0, xlength)] = INFLOW;
			flagField[computeFieldIndex(x, y, xlength[2] + 1, xlength)] = OUTFLOW;
		}
	}

	/* Boundary condition on X = 0 and X = Xmax is set OUTFLOW */
	for ( unsigned z = 0; z < xlength[2] + 2; ++z) {
		for ( unsigned y = 0; y < xlength[1] + 2; ++y) {
			flagField[computeFieldIndex(xlength, 0, y, z)] = NO_SLIP;
			flagField[computeFieldIndex(xlength, xlength[0] + 1, y, z)] = NO_SLIP;
		}
	}

	/* Boundary condition on Y = 0 and Y = Ymax is set NO_SLIP */
	for ( unsigned z = 0; z < xlength[2] + 2; ++z) {
		for ( unsigned x = 0; x < xlength[0] + 2; ++x) {
			flagField[computeFieldIndex(x, 0, z, xlength)] = NO_SLIP;
			flagField[computeFieldIndex(x, xlength[1] + 1, z, xlength)] = NO_SLIP;
		}
	}

	//TODO: Do we need some sort of check to constrain xlen. e.g. if xlen>>zlen
	/* Obstacle Definition and setting the boundary condtion to NO_SLIP*/
	for ( unsigned z = 0; z < (xlength[2] + 2) / 2; ++z) {
		for ( unsigned y = 0; y < xlength[1] + 2; ++y) {
			for ( unsigned x = 0; x < (xlength[0] + 2) / 2; ++x) {
				flagField[computeFieldIndex (x, y, z, xlength)] = NO_SLIP;
			}
		}
	}
