# Include files
SOURCES=initLB.cpp visualLB.cpp boundary.cpp collision.cpp streaming.cpp\
 		computeCellValues.cpp main.cpp helper.cpp DataStructure.cpp

# Compiler
# --------
CC=mpic++

#CFLAGS= -pedantic -Wall -g -fno-omit-frame-pointer -std=c++11#-Werror
CFLAGS= -pedantic -Wall -O3 -funroll-loops --std=c++11#Werror


#MODE=-DDEBUGGING
MODE=-DRELEASE
#MODE=-DMLUPS_FLAG
# Linker flags
# ------------
LDFLAGS= -lm -fopenmp

# ASSEMBLY FOLDER
FOLDER=./build/

OBJECTS=$(SOURCES:.cpp=.o)
OBJS = $(patsubst %,$(FOLDER)%,$(OBJECTS))

EXECUTABLE=lbsim

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXECUTABLE)


$(OBJECTS): %.o : %.cpp
	$(CC) $(CFLAGS) $(MODE) -c $< -o $(FOLDER)$@
