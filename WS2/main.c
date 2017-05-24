#ifndef _MAIN_C_
#define _MAIN_C_

#include <time.h>
#include <list>
#include <iostream>
#include <stdio.h>


#include "LBDefinitions.h"
#include "collision.h"
#include "streaming.h"
#include "initLB.h"
#include "visualLB.h"
#include "boundary.h"
#include "DataStructure.h"
#include "visualLB.h"


int main(int argc, char *argv[]){


    // check the correctness of the input parameters
     //
     // stop the execution if:
     //
     // 1. the number of input parameters is not equal to two
     //    ( including both the program name and the name of the input data file )
     //
     // 2. there is no such file at the project folder
     //
     if ( argc == 2 ) {

         // check if it's possible to open the file
         FILE* file;
         file = fopen( argv[1], "r" );

         // Abort execution if it's not possible to open the file
         if ( file == 0 ) {
             printf( "\nERROR: The file comtaining the input data doesn't exsist\n\n" );
             printf( "HINT:   Check the file spelling or its existence and try again\n\n");

             return -1;
         }

     }
     else {

         // Abort execution if the number of input parameters is wrong
         printf( "\nERROR: The number of input parameters is wrong\n\n" );

         printf( "HINT:   You've probably forgot to pass the input file" );
         printf( " as a parameter\n" );
         printf( "\tor you've passed more than one parameter\n\n" );

         printf( "Example: ./sim cavity100.dat\n\n" );
         return -1;
     }


//******************************************************************************

  // Allocate all fields
  int xlength = 5;
  int TotalLength = xlength + 2;
  int CellNumber = TotalLength * TotalLength * TotalLength;

  double *collideField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  double *streamField = ( double* )calloc( Vel_DOF * CellNumber, sizeof( double ) );
  int *flagField = ( int* )calloc( CellNumber, sizeof(int) );

  double wallVelocity[ 3 ] = { 1.0 ,1.0, 0.0 };

  // Allcocate the list of boundary layer cells
  std::list<Fluid*> BoundaryList;


  double Time = 0.0;
  const char* OUTPUT_FILE_NAME = "LBM";

  // initialize all fields

//******************************************************************************
//                          DEBUGGING: start
//******************************************************************************

// BEFORE WRITING THE MAIN LOOP ALL FUNCTIONS HAVE TO BE DEBUGGED !!!

  initialiseFields( collideField,
                    streamField,
                    flagField,
                    xlength );


  scanBoundary( BoundaryList,
              flagField,
              xlength,
              wallVelocity );


   writeVtkOutput( collideField,
                   OUTPUT_FILE_NAME,
                   Time,
                   xlength );



//******************************************************************************
//                          DEBUGGING: end
//******************************************************************************
   // Initializing timer
   clock_t Begin = clock();




    //**************************************************************************
   //
  //                            WHILE LOOP
 //
//******************************************************************************



   // display the output information
   clock_t End = clock();
   double ConsumedTime = (double)( End - Begin ) / CLOCKS_PER_SEC;

   std::cout << "Computational time: " << ConsumedTime << " sec " << std::endl;
   std::cout << "Mesh size: " << xlength << " x "
                              << xlength << " x "
                              << xlength << std::endl << std::endl;



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
