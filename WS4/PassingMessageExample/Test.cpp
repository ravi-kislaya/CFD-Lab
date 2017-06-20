#include <iostream>
#include <stdlib.h>
#include "../LBDefinitions.h"

unsigned computeCPUCoordinateX( unsigned iProc,
                                unsigned jProc,
                                unsigned kProc,
                                unsigned Rank ) {

	return ( unsigned ) ( Rank % iProc );

}

unsigned computeCPUCoordinateY( unsigned iProc,
                                unsigned jProc,
                                unsigned kProc,
                                unsigned Rank ) {

	return ( ( ( unsigned ) ( Rank / iProc ) ) % jProc );
}


unsigned computeCPUCoordinateZ( unsigned iProc,
                                unsigned jProc,
                                unsigned kProc,
                                unsigned Rank ) {

	return ( unsigned ) ( Rank / ( iProc * jProc ));
}


int main( int argc, char** argv ) {
    unsigned iProc = 10;
    unsigned jProc = 1;
    unsigned kProc = 1;

    unsigned ProcX = -1;
    unsigned ProcY = -1;
    unsigned ProcZ = -1;



    for ( unsigned i = 0; i < 10; ++i ) {

        std::cout << "Rank " << i << "   "
                  << "ProcX: " << computeCPUCoordinateX( iProc, jProc, kProc, i ) << " "
                  << "ProcY: " << computeCPUCoordinateY( iProc, jProc, kProc, i ) << " "
                  << "ProcZ: " << computeCPUCoordinateZ( iProc, jProc, kProc, i ) << " "
                  << std::endl;
    }


    return 0;
}
