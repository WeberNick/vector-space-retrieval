cmake_minimum_required(VERSION 3.9)
 
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
 
project(evsr VERSION 0.0.1)
 
#For Testing
include(CTest)
enable_testing()
 
include_directories(src/infra)
 
SET(SOURCE_FILES main.cpp)
add_executable(${CMAKE_PROJECT_NAME}_run ${SOURCE_FILES})
 
include_directories(src)
 
add_subdirectory(src)
add_subdirectory(tests)
 
target_link_libraries(${CMAKE_PROJECT_NAME}_run ${CMAKE_PROJECT_NAME}_lib)
 
 
 
# Doxygen Build
