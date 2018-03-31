project(${CMAKE_PROJECT_NAME}_lib)
 
set(HEADER_FILES
        argbase.hh
        args.hh
        document_manager.hh
        document.hh
        inverted_index.hh
        measure.hh
        tiered_index.hh
        infra/similarity_measures.hh
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
        util/utils.cc
)
 
#if (EXISTS ${SOURCE_FILES})
# The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#else()
# The library only contains header files.
#   add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
#endif()