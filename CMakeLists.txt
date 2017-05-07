# ceck the minimum version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


# set all project settings
PROJECT(TestLib)


# set source and include directories
SET(SOURCE_PATH ./googletest-release-1.8.0/googletest/)
SET(INCLUDE_PATH ./googletest-release-1.8.0/googlemock/include/)


# set file variables that have to be compiled
LIST(APPEND FILES ${SOURCE_PATH}src/gtest_main.cc 
                  ${SOURCE_PATH}src/gtest-all.cc)


# set up include directories
INCLUDE_DIRECTORIES(${INCLUDE_PATH})
INCLUDE_DIRECTORIES(${SOURCE_PATH})

# make a static library ( *.o )
ADD_LIBRARY(${PROJECT_NAME} STATIC ${FILES})


