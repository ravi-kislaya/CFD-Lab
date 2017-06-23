# Include files
SOURCES=initLB.cpp visualLB.cpp boundary.cpp collision.cpp streaming.cpp\
 		computeCellValues.cpp main.cpp helper.cpp DataStructure.cpp

# Compiler
# --------
CC=mpic++

#CFLAGS= -Wall -g -fno-omit-frame-pointer #-Werror
CFLAGS= -Wall -O2 -funroll-loops #-pedantic #-Werror


#MODE=-DDEBUGGING
#MODE=-DRELEASE
#MODE=-DTEST
#MODE=-DMLUPS_FLAG
# Linker flags

# ------------
LDFLAGS=

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
