cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(evsr VERSION 0.0.1)

#For Testing
include(CTest)
enable_testing()

SET(SOURCES src/main/main.cc src/document.cc src/document_manager.cc src/inverted_index.cc src/posting.cc)
SET(HEADERS src/infra/types.hh src/document.hh src/document_manager.hh src/inverted_index.hh src/posting.hh)

add_executable(evsr ${SOURCES} ${HEADERS})

#For Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
