include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Unit_Tests_run test_similarity_measures.cpp test_utils.cpp test_random_projection.cpp test_string_utils.cpp test_document.cpp)

target_link_libraries(Unit_Tests_run gtest gtest_main)
target_link_libraries(Unit_Tests_run ${CMAKE_PROJECT_NAME}_lib stdc++fs)

add_test(TEST ${CMAKE_HOME_DIRECTORY}/bin/Unit_Tests_run)