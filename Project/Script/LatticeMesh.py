from VertexClass import *
from copy import deepcopy

class CorruptedLattice(Exception):
    pass


class Lattice:
    __NUMBER_OF_FLUID_NEIGHBORS = 26


    def __init__(self, Coordinates, ID):
        self.ID = ID
        self.CoordinateList = Coordinates
        self.Neighbors = [  ]


    def setBoudary(self, Boundary):
        self.Boundary = Boundary


    def addNeighbors(self, Neighbors ):
        self.Neighbors.extend( Neighbors )


    def filter(self):
        # remove all redundant elements
        self.Neighbors = list( set( self.Neighbors ) )
        pass


    def printInfo( self ):
        if self.Boundary == 'Fluid':

            # check correctness of the lattice
            if len( self.Neighbors ) != Lattice.__NUMBER_OF_FLUID_NEIGHBORS:
                raise CorruptedLattice("more or less than 26 neighbors")

            print "Coordinates: ", self.CoordinateList, "Boudary: ", \
                self.Boundary, "Number Neighbors: ", len(self.Neighbors)


    def completeConstruction( self, Points ):

        SortableNeighbors = []

        # create a sortable list
        for ID in self.Neighbors:
            LatticeUnits = getLatticeUnits( self.CoordinateList, Points[ ID ] )

            SortableNeighbors.append( Vertex( deepcopy( ID ),
                                              LatticeUnits ) )

        # sort with respect to the lattic units
        InsertionSort3D( SortableNeighbors )



        # change the neighnor indeces with respect to sorting procedure
        for i in range( 0, len( self.Neighbors ) ):
            self.Neighbors[ i ] = SortableNeighbors[ i ].ID




################################################################################
#                               HELPER FUNCTIONS
################################################################################
def InsertionSort3D( SortableNeighbors ):

    # Sort with respect to Z
    for i in range(1, len( SortableNeighbors )):
        j = i-1
        Key = SortableNeighbors[ i ]
        while ( SortableNeighbors[ j ].Coordinates[ 2 ] < Key.Coordinates[ 2 ] ) \
                and ( j >= 0 ):

           SortableNeighbors[ j+1 ] = SortableNeighbors[ j ]
           j -= 1

        SortableNeighbors[j+1] = Key


    # Sort with respect to Y
    for i in range(1, len( SortableNeighbors )):
        j = i-1
        Key = SortableNeighbors[ i ]
        while ( SortableNeighbors[ j ].Coordinates[ 1 ] < Key.Coordinates[ 1 ] ) \
                and ( j >= 0 ):

           SortableNeighbors[ j+1 ] = SortableNeighbors[ j ]
           j -= 1

        SortableNeighbors[j+1] = Key


    # Sort with respect to X
    for i in range(1, len( SortableNeighbors )):
        j = i-1
        Key = SortableNeighbors[ i ]
        while ( SortableNeighbors[ j ].Coordinates[ 0 ] < Key.Coordinates[ 0 ] ) \
                and ( j >= 0 ):

           SortableNeighbors[ j+1 ] = SortableNeighbors[ j ]
           j -= 1

        SortableNeighbors[j+1] = Key



################################################################################


def getLatticeUnits( ReferencePoint, CurrentPoint ):
    DIMENSION = 3

    Accuracy = 1.0e-6

    # compute lattice stencil for the given lattice ant its neighbors
    UnitCoordinates = []

    # compute dimensional lattice coordinates
    for j in range( 0, DIMENSION ):

        CoordinateDifference =  CurrentPoint[ j ] - ReferencePoint[ j ]

        if CoordinateDifference > Accuracy:
            UnitCoordinates.append( 1 )

        elif -Accuracy > CoordinateDifference:
            UnitCoordinates.append( -1 )

        else:
            UnitCoordinates.append( 0 )

    return  UnitCoordinates

