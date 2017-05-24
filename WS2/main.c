#ifndef _MAIN_C_
#define _MAIN_C_

#include "LBDefinitions.h"
#include "collision.h"
#include "streaming.h"
#include "initLB.h"
#include "visualLB.h"
#include "boundary.h"
#include "DataStructure.h"
#include <list>
#include "iostream"

// DEBUGGING
#include <stdio.h>

int main(int argc, char *argv[]){
  /* TODO */

  std::list<Fluid*> BoundaryList;

  // Allocate all fields

  int xlength = 5;
  int TotalLength = xlength + 2;
  int CellNumber = TotalLength * TotalLength * TotalLength;
  double *collideField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  double *streamField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  int *flagField = ( int* )calloc( CellNumber, sizeof(int) );
  double wallVelocity[ 3 ] = { 1.0 ,1.0, 1.0 };

  // initialize all fields

  initialiseFields( collideField,
                    streamField,
                    flagField,
                    xlength );


  scanBoundary( BoundaryList,
              flagField,
              xlength,
              wallVelocity );

   std::cout << BoundaryList.size() << std::endl;



    // delete list of obstacles
    for ( std::list<Fluid*>::iterator Iterator = BoundaryList.begin();
          Iterator != BoundaryList.end();
          ++ Iterator ) {
        // clean all obstacles assigned to the fluid cell
        (*Iterator)->deleteObstacles();

        // delete all fluid cells
        delete (*Iterator);
    }

    // delete flields
    free( collideField );
    free( streamField );
    free( flagField );
  return 0;
}

#endif
