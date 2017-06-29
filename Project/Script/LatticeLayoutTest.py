

from LatticeSchemes import Stencil_D3Q26, Stencil_D3Q19
from LatticeMesh import getLatticeUnits

# define error if the stencil is violated
class CorruptedStencil(Exception):
    pass



def checkStencil_D3Q26( Lattice, Points ):
    Layout = []
    Accuracy = 1.0e-6

    # compute lattice stencil for the given lattice ant its neighbors
    for i in Lattice.Neighbors:

        UnitCoordinates = getLatticeUnits( Lattice.CoordinateList,
                                           Points[ i ])

        Layout.append( UnitCoordinates )


    # compare the computed stencil with the standart ( D3Q26 )
    # IMPORTANT: we're going compare only fluid elements
    try:
        for i in range( 0, len(Layout) ):

            if ( ( Lattice.Boundary == 'Fluid' )
                 and ( Stencil_D3Q26[ i ] != Layout[ i ] ) ):

                # print useful information with regards to the lattice stencil
                print
                print "expected pattern:", Stencil_D3Q26[ i ]
                print "gotten pattern:", Layout[ i ]

                # throw an exeption
                raise CorruptedStencil("Stencil was violated")

    except CorruptedStencil:

        # print useful information with regards to the lattice structure
        print "Layout is the following:"
        print Layout
        print "Number of element in the lattice: ", len( Lattice.Neighbors )
        print "Boundary conditions: ", Lattice.Boundary
        print

        # propogate the error
        raise CorruptedStencil("Stencil D3Q26 was violated")



def checkStencil_D3Q19( Lattice, Points):
    Layout = []
    Accuracy = 1.0e-6

    # compute lattice stencil for the given lattice ant its neighbors
    for i in Lattice.Neighbors:

        UnitCoordinates = getLatticeUnits( Lattice.CoordinateList,
                                           Points[ i ])
        Layout.append( UnitCoordinates )


    # compare the computed stencil with the standart ( D3Q26 )
    # IMPORTANT: we're going compare only fluid elements
    try:
        for i in range( 0, len(Layout) ):

            if ( ( Lattice.Boundary == 'Fluid' )
                 and ( Stencil_D3Q19[i] != Layout[i] ) ):

                # print useful information with regards to the lattice stencil
                print
                print "expected pattern:", Stencil_D3Q19[ i ]
                print "gotten pattern:", Layout[ i ]

                # throw an exeption
                raise CorruptedStencil("Stencil was violated")

    except CorruptedStencil:

        # print useful information with regards to the lattice structure
        print "Layout is the following:"
        print Layout
        print "Number of element in the lattice: ", len( Lattice.Neighbors )
        print "Boundary conditions: ", Lattice.Boundary
        print

        # propogate the error
        raise CorruptedStencil("Stencil D3Q19 was violated")
