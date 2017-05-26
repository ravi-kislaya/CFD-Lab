# Include files
SOURCES=initLB.c visualLB.c boundary.c collision.c streaming.c computeCellValues.c main.c helper.c DataStructure.c

# Compiler
# --------
CC=g++

CFLAGS= -pedantic -Wall #-Werror

#MODE=-DDEBUGGING
MODE=-DRELEASE

# Linker flags
# ------------
LDFLAGS=

# ASSEMBLY FOLDER
FOLDER=./build/

OBJECTS=$(SOURCES:.c=.o)
OBJS = $(patsubst %,$(FOLDER)%,$(OBJECTS))

EXECUTABLE=lbsim

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXECUTABLE)


$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) $(MODE) -c $< -o $(FOLDER)$@
