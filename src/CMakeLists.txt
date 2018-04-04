cmake_minimum_required(VERSION 3.9)
 
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        argbase.hh
        types.hh
        args.hh
        exception.hh
        measure.hh
        utils.hh
        document.hh
        document_manager.hh
        inverted_index.hh
        tiered_index.hh
        similarity_measures.hh
        random_projection.hh
)
 
set(SOURCE_FILES
        args.cc
        exception.cc
        utils.cc
        document.cc
        document_manager.cc
        inverted_index.cc
        tiered_index.cc
        similarity_measures.cc
        random_projection.cc
)
 
#if (EXISTS ${SOURCE_FILES})
# The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#else()
# The library only contains header files.
#   add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()
