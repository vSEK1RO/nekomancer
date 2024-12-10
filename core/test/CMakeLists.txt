set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

find_package(GTest QUIET)
if (NOT GTest_FOUND)
    message(STATUS "gtest not found, fetching...")
    FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
    )
    FetchContent_MakeAvailable(gtest)
else()
    message(STATUS "Found gtest")
endif()

enable_testing()

add_executable(
    nek-core-test
    test-main.cpp
)
target_include_directories(
    nek-core-test
    PRIVATE
    ../include
    .
)
target_link_libraries(
    nek-core-test
    PRIVATE
    GTest::gtest_main
    nek-core
)

include(GoogleTest)
gtest_discover_tests(nek-core-test)