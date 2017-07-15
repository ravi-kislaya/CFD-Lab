# Include files
SOURCES=initLB.cpp visualLB.cpp boundary.cpp collision.cpp streaming.cpp\
 		computeCellValues.cpp main.cpp helper.cpp DataStructure.cpp

# Compiler
# --------
<<<<<<< HEAD
CC=g++

#CFLAGS= -pedantic -Wall -g -fno-omit-frame-pointer -std=c++11#-Werror
CFLAGS= -pedantic -Wall -O3 -funroll-loops -std=c++11
=======
CC=mpic++
>>>>>>> 693b5d11e4cc6cd957cd3ea45e9ee9086174554d

#CFLAGS= -pedantic -Wall -g -fno-omit-frame-pointer -std=c++11#-Werror
CFLAGS= -pedantic -Wall -O3 -funroll-loops --std=c++11#Werror

#MODE= -DMLUPS_FLAG
#MODE=-DDEBUGGING
<<<<<<< HEAD
#MODE=-DRELEASE
=======
#MODE= -DRELEASE
>>>>>>> 693b5d11e4cc6cd957cd3ea45e9ee9086174554d
MODE=-DMLUPS_FLAG
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
