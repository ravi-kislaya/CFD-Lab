import numpy as np
import os
from copy import deepcopy

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

from LatticeMesh import Lattice
from ControlVolume import Cell
from LatticeLayoutTest import *

NEW_LINE = '\n'
BLOCK_BEGIN = '('
BLOCK_END = ')'
CHUNK_END = '}'


################################################################################
#                                   MAIN
################################################################################
def main():

    Points = readPoints()

    # initialize the lattice mesh
    ID = 0
    LatticeMesh = []
    for Entry in Points:
        LatticeMesh.append( Lattice( Entry, ID ) )
        ID += 1


    # Read faces and boundaries
    Faces = readFaces()
    Boundaries = readBoundaries()


    # assign boundaris to corresponding points
    for i in range(1, len( Boundaries ) ):
        Range = [ Boundaries[ i - 1 ][ 1 ], Boundaries[ i ][ 1 ] ]

        assigneBoundary( LatticeMesh,
                         Boundaries[ i - 1 ][ 0 ],
                         Faces,
                         Range )

    # proceed the last boundary:
    # which should be the NO SLIP boundary consition
    # to overwrite violated faces
    Range = [Boundaries[ -1 ][ 1 ], len(Faces) ]
    assigneBoundary( LatticeMesh,
                     Boundaries[ -1 ][ 0 ],
                     Faces,
                     Range )



    # Read and construct cells
    Cells = readCells( Points, Faces )


    # insert points in cells
    for Cell in Cells:
        for i in Cell.Faces:

            # add points to a cell object
            Cell.addSetPoints( Faces[ i ] )

        # remove all repeats
        Cell.Points = list(set(Cell.Points))


    # assign all neighbors to the coresponding lattices
    for Cell in Cells:
        for i in range( 0, len( Cell.Points ) ):
            LatticeMesh[ Cell.Points[ i ] ].addNeighbors( Cell.Points[ :i ] )
            LatticeMesh[ Cell.Points[ i ] ].addNeighbors( Cell.Points[ i+1: ] )


    # remove all repeats in the neighbors list and sort neighbors
    for Element in LatticeMesh:
        Element.filter()
        Element.completeConstruction( Points )


    # check only fluid lattice elements and its neighbor with
    # respect to corectness of D3Q26 stencil
    for i in range( 0, len(LatticeMesh) ):
        checkStencil_D3Q26( LatticeMesh[ i ], Points )

    printAdjacencyList( LatticeMesh )


    print "Mesh was converted correctly to D3Q26 stencil"


    # convert the current stencil to D3Q19 scheme
    for Element in LatticeMesh:
        if Element.Boundary == 'Fluid':
            Element.Neighbors = convertToD3Q19( Element )



    # check only fluid lattice elements and its neighbor with
    # respect to corectness of D3Q19 stencil
    for i in range(0, len(LatticeMesh)):
        checkStencil_D3Q19( LatticeMesh[ i ], Points )



    print "Mesh was converted correctly to D3Q19 stencil"

    # write all files, namely: FlagField.fg Coordinates.crd, Neighbors.nb
    printExtendedResults( LatticeMesh, Boundaries, 'LatticeMesh' )



################################################################################
#                               HELPER FUNCTIONS
################################################################################
def readPoints():

    file = open('polyMesh/points', 'r')
    Text = file.read().splitlines()

    findEntryLine( Text )
    List = []


    for line in Text:
        if line != BLOCK_END:

            # get rid of brackets
            line = line.strip('()')

            # convert ot a list of words
            line = line.split(' ')

            line = map( float, line )

            # add to the list of points
            List.append( line )

        else:
            break

    return List

def readFaces():

    file = open('polyMesh/faces', 'r')
    Text = file.read().splitlines()

    findEntryLine(Text)
    Faces = []

    for line in Text:
        if line != BLOCK_END:

            # get rid of brackets
            line = line[1:]

            line = line.strip('( )')

            # convert ot a list of words
            line = line.split()

            # convert to a list of integers from a string
            line = map(int, line)


            # add to the list of points
            Faces.append( line )

        else:
            break

    return Faces

def readBoundaries():

    file = open('polyMesh/boundary', 'r')
    Text = file.read().splitlines()

    findEntryLine(Text)

    Boundaries = [ [ 'Fluid', 0 ] ]
    Entry = []

    for line in Text:
        if line != BLOCK_END:

            if CHUNK_END not in line:
                Entry.append( line )
            else:
                processEntry( Boundaries, Entry )
                Entry = []
        else:
            break

    return Boundaries

def readCells( Points, Faces ):

    Cells = []


    file = open('polyMesh/owner', 'r')
    Text = file.read().splitlines()

    CellNumber = 0
    isFound = False

    # Find the number of cells
    for line in Text:
        line = line.split(' ')

        # scan through the entire line to find "nCells:" entry
        for i in range( len( line ) ):
            if 'nCells:' == line[ i ]:

                # the next entry after "nCells:" represents
                # the total number of cells
                CellNumber = line[ i + 1 ]
                isFound = True
                break

        if isFound == True:
            break



    # convert the found string to integer
    CellNumber = int( CellNumber )


    # Find the entry of the block
    findEntryLine(Text)
    Cells = [ 0 ] * CellNumber


    # compile the cell array
    Counter = 0
    for line in Text:
        if line != BLOCK_END:

            ID = int(line)

            if Cells[ ID ] != 0:
                Cells[ ID ].addFaceID( Counter )

            else:
                Cells[ ID ] = Cell( ID )
                Cells[ ID ].addFaceID( Counter )

            Counter += 1


        else:
            break


        ################################################################################
    file = open('polyMesh/neighbour', 'r')
    Text = file.read().splitlines()

    # Find the entry of the block
    findEntryLine(Text)

    #for line in Text:
        #if line != BLOCK_END:

            #ID = int(line)
            #print ID, type( Cells[ID] )
            #if ( type( Cells[ID] ) == type( int ) ):
            #    print "Error"



    Counter = 0
    for line in Text:
        if line != BLOCK_END:

            ID = int(line)
            Cells[ID].addFaceID(Counter)
            Counter += 1

        else:
            break



    return Cells

def processEntry( Boundaries, Entry ):


    # the first line is the name of a boundary
    Temp = [ Entry[0].lstrip() ]


    # the startFace is the beginning index of the boundary set
    for String in Entry:
        if 'startFace' in String:
            Info = String
            break

    # remove all redundant characters
    Info = Info.strip(';')
    Info = Info.split(' ')

    Temp.append( int(Info[-1]) )
    # add a new boundary to the list
    Boundaries.append( Temp )

def assigneBoundary( Mesh, SpecificBoundary, Faces, Range ):
    # @param:
    #   Mesh - the entire list of lattice points ( coordinates + ID )
    #   SpecificBoundary - name of the boundary i.e. outlet, inlet, wall etc.
    #   Faces - the entire list of faces... Look at the openFoam documentation
    #   Range - range in the face list. The range tells what faces belong to
    #           to the given BC
    # Function - Considering the given range of faces the function assignes
    #            points to the specific boundary
    for i in range( Range[0], Range[1] ):
        for Point in Faces[ i ]:
            Mesh[ Point ].setBoudary( SpecificBoundary )

def findEntryLine( Text ):

    while True:

        if Text[ 0 ] != BLOCK_BEGIN:
            del Text[ 0 ]
        else:
            break

    del Text[0]

def convertToD3Q19( LaticeElemet ):
    # IMPORTANT: the 19th point is needed for representation purpose
    # you don't have to use it in physics simulation
    # The point is only needed to depict a fluid cell

    D3Q19_Neighbors = [ LaticeElemet.Neighbors[16],  #0
                        LaticeElemet.Neighbors[22],  #1
                        LaticeElemet.Neighbors[13],  #2
                        LaticeElemet.Neighbors[5],   #3
                        LaticeElemet.Neighbors[11],  #4
                        LaticeElemet.Neighbors[24],  #5
                        LaticeElemet.Neighbors[15],  #6
                        LaticeElemet.Neighbors[7],   #7
                        LaticeElemet.Neighbors[21],  #8
                        LaticeElemet.ID,             #9
                        LaticeElemet.Neighbors[4],   #10
                        LaticeElemet.Neighbors[18],  #11
                        LaticeElemet.Neighbors[10],  #12
                        LaticeElemet.Neighbors[1],   #13
                        LaticeElemet.Neighbors[14],  #14
                        LaticeElemet.Neighbors[20],  #15
                        LaticeElemet.Neighbors[12],  #16
                        LaticeElemet.Neighbors[3],   #17
                        LaticeElemet.Neighbors[9],   #18
                        LaticeElemet.Neighbors[0] ]  #19

    return D3Q19_Neighbors

def plotBoundary( LatticeMesh, Boundary, Plot, Color ):
    X = []
    Y = []
    Z = []



    for Lattice in LatticeMesh:
        if Lattice.Boundary == Boundary:
            X.append( Lattice.CoordinateList[0] )
            Y.append( Lattice.CoordinateList[1] )
            Z.append( Lattice.CoordinateList[2] )

    Plot.scatter( X, Y, Z, c = Color, marker = 'o' )

def printResults( LatticeMesh, Boundaries, FileName ):

    FlagFieldFileName = FileName + '-FlagField.fg'
    LatticeFieldFileName = FileName + '.3r'

    FlagFieldFile = open( FlagFieldFileName, 'w' )

    # write header for the flag filed file
    FlagFieldFile.write( 'Number of Elements: %d\n\n' % len( LatticeMesh ) )

    # write data info to the flag field file
    for Entry in LatticeMesh:
        FlagFieldFile.write('%s\n' % getFlagFieldString( Entry ) )


    # prepare data for the lattice field file
    FluidCell = []
    for Elemet in LatticeMesh:
        if Elemet.Boundary == 'Fluid':
            FluidCell.append( getFluidInfoAsString( Elemet ) )

    LatticeFieldFile = open(LatticeFieldFileName, 'w')

    # write header for the lattice field file
    LatticeFieldFile.write('Total Number of Elements: %d\n' % len(LatticeMesh) )
    LatticeFieldFile.write('Number of Fluid Elements: %d\n\n' % len(FluidCell) )

    # write data info to the lattice field file
    for Cell in FluidCell:
        LatticeFieldFile.write('%s\n' % Cell)
        pass


def printAdjacencyList( LatticeMesh ):

    # compute the number of edges
    NumberOfEdges = 0
    for Cell in LatticeMesh:
        NumberOfEdges += len( Cell.Neighbors )

    NumberOfEdges = NumberOfEdges / 2


    ########################     write adjacency list  #########################
    AdjacencyListFileName = 'Output/AdjacencyList.graph'
    AdjacencyFile = open( AdjacencyListFileName, 'w' )

    AdjacencyFile.write( '%d %d\n' % (len( LatticeMesh ), NumberOfEdges) )

    for Cell in LatticeMesh:

        for i in range( len( Cell.Neighbors ) ):
            AdjacencyFile.write( ' %d' % ( Cell.Neighbors[ i ] + 1 ) )

        AdjacencyFile.write( '\n' )

def printExtendedResults( LatticeMesh, Boundaries, FileName ):

    # separator is a set of characters that separates a file heade and data
    SEPARATOR = "############################"

    ################     write flags to the corresponding file  ################
    FlagFieldFileName = 'Output/FlagField.fg'

    FlagFieldFile = open( FlagFieldFileName, 'w' )

    FlagFieldFile.write( 'Number of Elements: %d\n\n' % len( LatticeMesh ) )
    FlagFieldFile.write( '%s\n' % SEPARATOR )

    for Entry in LatticeMesh:
        FlagFieldFile.write('%s\n' % getFlagFieldString( Entry ) )



    ##########     write all coordinates into the corresponding file  ##########
    CoordinatesFileName = 'Output/Coordinates.crd'

    FileOfCoordinates = open(CoordinatesFileName, 'w')

    # write header for the lattice coordinates file
    FileOfCoordinates.write('Number of Elements: %d\n\n' % len(LatticeMesh))
    FileOfCoordinates.write( '%s\n' % SEPARATOR )

    FlagFieldFile.close()

    for Cell in LatticeMesh:
        FileOfCoordinates.write('%d ' % Cell.ID)
        FileOfCoordinates.write('%f ' % Cell.CoordinateList[ 0 ] )
        FileOfCoordinates.write('%f ' % Cell.CoordinateList[ 1 ] )
        FileOfCoordinates.write('%f\n' % Cell.CoordinateList[ 2 ] )

    FileOfCoordinates.close()



    ##########     write all neighbours into the corresponding file  ##########
    NeighborFileName = 'Output/Neighbors.nb'

    FileOfNeighbors = open(NeighborFileName, 'w')

    # write header for the lattice neighbours file
    FileOfNeighbors.write('Number of Elements: %d\n\n' % len( LatticeMesh ))
    FileOfNeighbors.write( '%s\n' % SEPARATOR )

    for Cell in LatticeMesh:
        FileOfNeighbors.write( '%d ' % Cell.ID )

        if ( Cell.Boundary == 'Fluid' ):
            # if a cell represents a fluid element then all neighbours ID will
            # be printed out
            for i in range( len( Cell.Neighbors ) ):
                FileOfNeighbors.write('%d ' % Cell.Neighbors[ i ] )

            FileOfNeighbors.write('\n')

        else:
            # otherwise the NONE statment will be printed
            FileOfNeighbors.write( 'NONE\n' )

    FileOfNeighbors.close()


def getFluidInfoAsString( Fluid ):
    Info = []
    Info.append( Fluid.ID )
    Info.extend( Fluid.CoordinateList )
    Info.extend( Fluid.Neighbors )

    String = []
    for i in Info:
        String.append( str( i ) )


    return " ".join( String )


def getBoundaryInfoAsString( Obstacle ):
    Info = []
    Info.append( Obstacle.ID )
    Info.extend( Obstacle.CoordinateList )

    String = []
    for i in Info:
        String.append( str( i ) )

    return " ".join( String )


def getFlagFieldString( Entry ):

    Info = []
    Info.append( Entry.ID )
    Info.append( Entry.Boundary )

    String = []
    for i in Info:
        String.append( str( i ) )

    return " ".join( String )

main()