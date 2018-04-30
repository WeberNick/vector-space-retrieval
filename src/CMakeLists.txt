cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_lib)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

#Set Boost library paths
if(LINUX)
    set(BOOST_INCLUDE_DIR "/usr/include/boost")
    set(BOOST_LIBRARY_DIR "/usr/lib/x86_64-linux-gnu")
else()
    if(NOT EXISTS "$ENV{BOOST}")
        message(WARNING "Could not find BOOST environment variable")
        message("Try using the standard brew boost path")
        set(BOOST_ROOT "/usr/local/Cellar/boost/1.66.0/")
    else()
        message("Using provided BOOST path: /usr/local/Cellar/boost/1.66.0/")
        set(BOOST_ROOT $ENV{BOOST})
    endif()

    set(BOOST_INCLUDE_DIR "${BOOST_ROOT}/include")
    set(BOOST_LIBRARY_DIR "${BOOST_ROOT}/lib")
endif()

#Set src/lib as other external librs
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/src/lib/)

# Clone non-boost external libs
## Clone Eigen
if(NOT EXISTS ${EXTERNAL_DIR}eigen)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE}  clone https://github.com/eigenteam/eigen-git-mirror --branch 3.3.4 --single-branch ${EXTERNAL_DIR}eigen
    )
endif()

## Clone /OleanderStemmingLibrary
if(NOT EXISTS ${EXTERNAL_DIR}oleanderStemmingLibrary)
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} clone https://github.com/OleanderSoftware/OleanderStemmingLibrary.git ${EXTERNAL_DIR}oleanderStemmingLibrary
    )
endif()

## Clone /nlohmann (JSON)
if(NOT EXISTS ${EXTERNAL_DIR}nlohmann)
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/nlohmann/json.git ${EXTERNAL_DIR}nlohmann
    )
endif()

set(HEADER_FILES
        argbase.hh
        types.hh
        utility.hh
        args.hh
        exception.hh
        trace.hh
        measure.hh
        evaluation.hh
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
        trace.cc
        evaluation.cc
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

#Create library which is later linked to the main executable
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

#Include Boost libraries to target
set(BOOST_MIN_VERSION 1.58.0)
find_package (Boost ${BOOST_MIN_VERSION} REQUIRED)
if (Boost_FOUND)
    target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC SYSTEM ${BOOST_INCLUDE_DIR})
else()
    return()
endif ()

#Inlcude non-boost libraries
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC SYSTEM "${CMAKE_SOURCE_DIR}/src/lib/eigen")
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC SYSTEM "${CMAKE_SOURCE_DIR}/src/lib/oleanderStemmingLibrary")
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC SYSTEM "${CMAKE_SOURCE_DIR}/src/lib/nlohmann/single_include")
