include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)
enable_testing()

list(APPEND ${PROJECT_NAME}_test_SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/Octree.cpp")

add_executable(${PROJECT_NAME}_test ${${PROJECT_NAME}_test_SOURCES})

target_link_libraries(${PROJECT_NAME}_test PUBLIC gtest_main sourcepp)

target_include_directories(${PROJECT_NAME}_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
