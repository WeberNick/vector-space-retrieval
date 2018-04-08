cmake_minimum_required(VERSION 3.9)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)


set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-deprecated-declarations -Wno-deprecated -Wall -Weffc++")

set(CMAKE_VERBOSE_MAKEFILE ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)


project(evsr VERSION 0.0.1)

#For Testing
include(CTest)
enable_testing()

SET(SOURCE_FILES main.cc)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})

include_directories(src)
include_directories(src/infra)
include_directories(src/util)

add_subdirectory(src)
add_subdirectory(tests)
#add_subdirectory(docs)

target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)