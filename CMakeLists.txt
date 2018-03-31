cmake_minimum_required(VERSION 3.9)
 
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
 
project(evsr VERSION 0.0.1)
 
#For Testing
include(CTest)
enable_testing()
 
include_directories(src/infra)
include_directories(src/util)
 
SET(SOURCE_FILES main.cc)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})
 
include_directories(src)
 
add_subdirectory(src)
add_subdirectory(tests)
 
target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)



# Doxygen Build
#option(BUILD_DOC "Build Documentation" ON)

#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    set(BUILD_DOC_DIR ${CMAKE_SOURCE_DIR}/build/docs)
#    if(NOT EXISTS ${BUILD_DOC_DIR})
#        file(MAKE_DIRECTORY ${BUILD_DOC_DIR})
#    endif()

#    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#    message("Doxygen build started")
#    add_custom_target(Doxygen ALL
#            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#            COMMENT "Generating API documentation with Doxygen"
#            VERBATIM)
#else(DOXYGEN_FOUND)
#    message("Doxygen needs to be installed to generate the documentation.")
#endif(DOXYGEN_FOUND)
