set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-unused-result -O2")

include(FetchContent)

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, fetching...")
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
else()
    message(STATUS "Found nlohmann_json")
endif()

add_library(
    nek-core
    SHARED
    Reactive.cpp
    Exception.cpp
    Json.cpp
)
target_link_libraries(
    nek-core
    PUBLIC
    nlohmann_json
)
target_include_directories(
    nek-core
    PUBLIC
    ../include
)