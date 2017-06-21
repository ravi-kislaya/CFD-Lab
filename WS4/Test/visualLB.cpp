#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#include "helper.h"
#include "visualLB.h"
#include "computeCellValues.h"
#include "LBDefinitions.h"
#include "DataStructure.h"



//Writes Density and Velocity from the collision field
void writeVtkOutput( const char * filename,
                     int * const flagField,
                     std::vector<Fluid*>& FluidDomain,
                     std::list<Fluid*>& VTKrepresentation,
                     int *IdField,
                     unsigned int t,
                     unsigned* Length ) {


    char szFilename[80];
    FILE *fp = NULL;
    sprintf( szFilename, "%s.%i.vtk", filename, t );

    fp = fopen( szFilename, "w");
    if( fp == NULL ) {
        char szBuff[80];
        sprintf( szBuff, "Failed to open %s", filename );
        ERROR(szBuff);
        return;
    }

//------------------------------------------------------------------------------
//                   Write the file header and coordinates
//------------------------------------------------------------------------------

    write_vtkHeader( fp, FluidDomain, Length );

    write_vtkPointCoordinates( fp, FluidDomain, Length );
//------------------------------------------------------------------------------
//                   Write the file header and coordinates
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
    fprintf(fp, "\nPOINT_DATA %lu \n", (Length[0]+2) * (Length[1]+2) * (Length[2]+2) );



//------------------------------------------------------------------------------
//                        Write density to the file
//------------------------------------------------------------------------------
    //Computing Density
    fprintf(fp, "SCALARS density int 1 \n");
    fprintf(fp, "LOOKUP_TABLE default \n");

    // DEBUGGING: chech computeDensity

#ifdef DEBUGGING
    const double MAXIMUM_DENSITY = 1.1;
    const double MINIMUM_DENSITY = 0.9;
#endif


    int Index = 0;
    for ( unsigned z = 0; z < Length[ 2 ] + 2; ++z ) {
		for ( unsigned y = 0; y < Length[ 1 ] + 2; ++y ) {
			for ( unsigned x = 0; x < Length[ 0 ] + 2; ++x ) {

                Index = flagField [ computeFlagIndex( x, y, z, Length )  ];
                //computeDensity ( Index, &Density );
				//if(Index == 150)
					//printf( "%d %d %d\n", x,y,z );
                fprintf( fp, "%d\n", Index );

	}}}

    fclose(fp);

}


void write_vtkHeader( FILE *fp,
                      std::vector<Fluid*>& FluidDomain,
                      unsigned* Length ) {

    if( fp == NULL )
    {
        char szBuff[80];
        sprintf( szBuff, "Null pointer in write_vtkHeader" );
        ERROR( szBuff );
        return;
    }

    fprintf(fp,"# vtk DataFile Version 2.0\n");
    fprintf(fp,"generated by CFD-lab course output \n");
    fprintf(fp,"ASCII\n");
    fprintf(fp,"\n");
    fprintf(fp,"DATASET STRUCTURED_GRID\n");
    fprintf(fp,"DIMENSIONS  %i %i %i \n", Length[0]+2, Length[1]+2, Length[2]+2);
    fprintf(fp,"POINTS %i float\n", (Length[0]+2) * (Length[1]+2) * (Length[2]+2) );
    fprintf(fp,"\n");

}


void write_vtkPointCoordinates( FILE* fp,
                                std::vector<Fluid*>& FluidDomain,
                                unsigned* Length ) {

    for(unsigned z = 0; z<Length[2]+2; ++z) {
		for(unsigned y = 0; y<Length[1]+2; ++y) {
			for(unsigned x = 0; x<Length[0]+2; ++x) {
				fprintf(fp, "%d %d %d\n", x, y, z );
			}
		}
	}


}


