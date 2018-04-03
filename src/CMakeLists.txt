project(${CMAKE_PROJECT_NAME}_lib)

# Clone Eigen
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/src/lib/)
if(NOT EXISTS ${EIGEN_DIR})
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/eigenteam/eigen-git-mirror ${EXTERNAL_DIR}eigen
    )
endif()

set(HEADER_FILES
        argbase.hh
        args.hh
        document_manager.hh
        document.hh
        inverted_index.hh
        measure.hh
        tiered_index.hh
        infra/similarity_measures.hh
        infra/random_projection.h
        infra/types.hh
        util/utils.hh
)

set(SOURCE_FILES
        args.cc
        document_manager.cc
        document.cc
        inverted_index.cc
        tiered_index.cc
        infra/similarity_measures.cc
        infra/random_projection.cc
        util/utils.cc
)

include_directories(infra)
include_directories(util)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/eigen/Eigen)

#add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/eigen)

#find_package (Eigen3 3.3 REQUIRED NO_MODULE)


add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})






