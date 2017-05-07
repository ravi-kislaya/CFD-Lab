CC = g++


CFLAGS = -lgtest -lpthread

TARGET = TEST

FILES = UnitTest.cpp Matrix.cpp ../helper.c libGoogleTest.a

PROJECT_INCLUDE = -I../
GOOGLE_INCLUDE = -I../../GoogleTestLib/googletest-release-1.8.0/googletest/include/

LIBRARY_PATH = -L./

all:
	$(CC) $(PROJECT_INCLUDE) $(GOOGLE_INCLUDE) $(LIBRARY_PATH) $(FILES) -o $(TARGET) $(CFLAGS)
	


clean: 
	rm -f ./*.o
