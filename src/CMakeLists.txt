project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES
        document.hh
        document_manager.hh
        inverted_index.hh
        posting.hh
        tiered_index.hh
        infra/similarity_measures.hh
        infra/types.hh
        infra/utils.hh
        )

set(SOURCE_FILES
        document.cc
        document_manager.cc
        inverted_index.cc
        posting.cc
        tiered_index.cc
        infra/similarity_measures.cc
        infra/utils.cc
        )

#if (EXISTS ${SOURCE_FILES})
# The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#else()
# The library only contains header files.
#   add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()