cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)

project(evsr VERSION 0.0.1)

#For Testing
include(CTest)
enable_testing()

add_executable(evsr src/main/main.cc src/infra/types.hh)

#For Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
