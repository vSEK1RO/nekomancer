include(FetchContent)

find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, fetching...")
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
    set(POCO_BUILD_CODERS OFF CACHE BOOL "Disable Poco coders" FORCE)
    set(POCO_BUILD_NET OFF CACHE BOOL "Disable Poco network modules" FORCE)
    set(POCO_BUILD_UTIL OFF CACHE BOOL "Disable Poco utility modules" FORCE)
    set(POCO_BUILD_XML OFF CACHE BOOL "Disable Poco XML modules" FORCE)
    set(POCO_BUILD_NETSSL OFF CACHE BOOL "Disable Poco SSL/TLS support" FORCE)
    set(POCO_BUILD_TESTS OFF CACHE BOOL "Disable Poco tests" FORCE)
    set(POCO_BUILD_EXAMPLES OFF CACHE BOOL "Disable Poco examples" FORCE)
    message(STATUS "Poco not found, fetching...")
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
set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")

add_library(
    nek-core
    SHARED
    Reactive.cpp
    Exception.cpp
    Json.cpp
    Component/Manager.cpp
    Component/Store.cpp
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