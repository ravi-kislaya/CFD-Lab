class Vertex:
    def __init__(self, ID, Coordinates):
        self.ID = ID
        self.Coordinates = Coordinates

    def printInfo(self):
        print "ID: ", self.ID, "Coordinates", self.Coordinates
