cmake_minimum_required(VERSION 3.9)
 
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(${CMAKE_PROJECT_NAME}_lib)

# Clone Eigen
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/src/lib/)
if(NOT EXISTS ${EIGEN_DIR})
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/eigenteam/eigen-git-mirror ${EXTERNAL_DIR}eigen
    )
endif()

include_directories(./infra)
include_directories(./util)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/eigen/)

set(HEADER_FILES
        argbase.hh
        args.hh
        document_manager.hh
        document.hh
        inverted_index.hh
        measure.hh
        tiered_index.hh
        types.hh
        utility.hh
        random_projection.hh
        index_manager.hh
        posting_list.hh
        query_processing_engine.hh
)

set(SOURCE_FILES
        args.cc
        document_manager.cc
        document.cc
        inverted_index.cc
        tiered_index.cc
        random_projection.cc
        index_manager.cc
        posting_list.cc
        query_processing_engine.cc
)
 
#if (EXISTS ${SOURCE_FILES})
# The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#else()
# The library only contains header files.
#   add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()
