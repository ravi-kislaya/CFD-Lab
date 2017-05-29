# Include files
SOURCES=initLB.c visualLB.c boundary.c collision.c streaming.c\
 		computeCellValues.c main.c helper.c DataStructure.c

# Compiler
# --------
CC=g++

#CFLAGS= -pedantic -Wall -g -fno-omit-frame-pointer #-Werror
CFLAGS= -pedantic -Wall -O2 -funroll-loops -Werror


#MODE=-DDEBUGGING
MODE=-DRELEASE
#MODE=-DMLUPS_FLAG
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
