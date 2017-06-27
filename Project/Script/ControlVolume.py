class CorruptedCell(Exception):
    pass


class Cell:
    __OCTAVE = 8

    def __init__(self, ID):
        self.CellID = ID
        self.Faces = [ ]
        self.Points = [ ]


    def addFaceID(self, Face):
        self.Faces.append( Face )


    def addSetPoints(self, Points):
        self.Points.extend( Points )


    def printInfo(self):
        Temp = len(self.Points)

        if Temp == Cell.__OCTAVE:
            print "ID: ", self.CellID, "Points: ", self.Points

        else:
            print "ERROR: ", self.CellID, "Points: ", self.Points
            raise CorruptedCell("The cell contains more or less than 8 vertexes")