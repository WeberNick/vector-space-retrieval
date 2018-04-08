cmake_minimum_required(VERSION 3.9)

find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Weffc++ -Wno-deprecated-declarations")

project(${CMAKE_PROJECT_NAME}_lib)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/src/lib/)

# Clone Eigen
if(NOT EXISTS ${EXTERNAL_DIR}eigen)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone https://github.com/eigenteam/eigen-git-mirror ${EXTERNAL_DIR}eigen
    )
endif()

# Clone /OleanderStemmingLibrary
if(NOT EXISTS ${EXTERNAL_DIR}oleanderStemmingLibrary)
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/OleanderSoftware/OleanderStemmingLibrary.git ${EXTERNAL_DIR}oleanderStemmingLibrary
    )
endif()

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/src/lib/eigen")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/src/lib/oleanderStemmingLibrary")

set(HEADER_FILES
        argbase.hh
        types.hh
        utility.hh
        args.hh
        exception.hh
        measure.hh
        document.hh
        document_manager.hh
        inverted_index.hh
        tiered_index.hh
        cluster.hh
        random_projection.hh
        index_manager.hh
        posting_list.hh
        query_processing_engine.hh
)

set(SOURCE_FILES
        args.cc
        exception.cc
        document.cc
        document_manager.cc
        inverted_index.cc
        tiered_index.cc
        cluster.cc
        random_projection.cc
        index_manager.cc
        posting_list.cc
        query_processing_engine.cc
)


add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
