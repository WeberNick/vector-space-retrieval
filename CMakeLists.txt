cmake_minimum_required(VERSION 3.9)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)


set (CMAKE_CXX_STANDARD 17)

set(CMAKE_VERBOSE_MAKEFILE ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wno-deprecated-declarations -Wall")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lstdc++fs")

project(evsr VERSION 0.0.1)

#For Testing
include(CTest)
enable_testing()

SET(SOURCE_FILES main.cc)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
#add_subdirectory(tests)
#add_subdirectory(docs)

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)

#NICK: Use this to supress deprecated warning in file
#src/lib/oleanderStemmingLibrary/stemming/../indexing/string_util.h
#line number 299 (remove first #):

# 	#pragma GCC diagnostic push
# 	#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
# 	int* digits = new int[length+1]; std::auto_ptr<int> digitsDeleter(digits); 
# 	#pragma GCC diagnostic pop
