set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-unused-result -O2")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

find_package(jsoncpp QUIET)
if (NOT jsoncpp_FOUND)
    message(STATUS "jsoncpp not found, fetching...")
    include(FetchContent)
    FetchContent_Declare(
        jsoncpp
        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
        GIT_TAG        1.9.6
    )
    FetchContent_MakeAvailable(jsoncpp)
else()
    message(STATUS "Found jsoncpp")
endif()

add_library(
    nek-core
    SHARED
    IJsonify.cpp
    Exception.cpp
)
target_link_libraries(
    nek-core
    PUBLIC
    jsoncpp_lib
)
target_include_directories(
    nek-core
    PUBLIC
    ../include
)