cmake_minimum_required(VERSION 3.9)

#Use standard gcc and g++ in env path if not else specified
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(evsr VERSION 0.0.1)

# Specifiy if makefile is verbose and set output directories
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)

#Set c++ standard, warning flags and linker flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wno-deprecated-declarations -Wno-deprecated -Wall -Wno-int-in-bool-context")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs")

#For Testing
include(CTest)
enable_testing()

#Set source files
SET(SOURCE_FILES main.cc)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})

include_directories(src)

#Add subdirectories and execute their CmakeLists
add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(docs)

#Link main executable with code lib
target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib stdc++fs)

#NICK: Use this to supress deprecated warning in file
#src/lib/oleanderStemmingLibrary/stemming/../indexing/string_util.h
#line number 299 (remove first #):

# 	#pragma GCC diagnostic push
# 	#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 	int* digits = new int[length+1]; std::auto_ptr<int> digitsDeleter(digits);
# 	#pragma GCC diagnostic pop