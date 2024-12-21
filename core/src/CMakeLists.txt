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

find_package(Poco QUIET)
if (NOT Poco_FOUND)
    message(STATUS "Poco not found, fetching...")
    include(FetchContent)
    FetchContent_Declare(
        Poco
        GIT_REPOSITORY https://github.com/pocoproject/poco.git
        GIT_TAG        poco-1.13.3-release
    )
    FetchContent_MakeAvailable(Poco)
else()
    message(STATUS "Found Poco")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-unused-result -O2")

add_library(
    nek-core
    SHARED
    Reactive.cpp
    Exception.cpp
    Json.cpp
    Component/Manager.cpp
)
target_link_libraries(
    nek-core
    PUBLIC
    nlohmann_json
    Poco::Foundation
)
target_include_directories(
    nek-core
    PUBLIC
    ../include
    ${Poco_INCLUDE_DIRS}
)
target_link_directories(
    nek-core
    PUBLIC
    ${Poco_LIBRARY_DIRS}
)