find_package(argparse QUIET)
if (NOT argparse_FOUND)
    message(STATUS "argparse not found, fetching...")
    FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    )
FetchContent_MakeAvailable(argparse)
else()
    message(STATUS "Found argparse")
endif()

add_executable(
    nek-engine
    main.cpp
)
target_link_libraries(
    nek-engine
    PRIVATE
    nek-core
)
target_include_directories(
    nek-engine
    PRIVATE
    ${argparse_INCLUDE_DIRS}
)