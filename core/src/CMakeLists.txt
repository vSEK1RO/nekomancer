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

find_package(nlohmann_json_schema_validator QUIET)
if (NOT nlohmann_json_schema_validator_FOUND)
    message(STATUS "nlohmann_json_schema_validator not found, fetching...")
    FetchContent_Declare(
        nlohmann_json_schema_validator
        GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
        GIT_TAG        2.3.0
    )
    FetchContent_MakeAvailable(nlohmann_json_schema_validator)
else()
    message(STATUS "Found nlohmann_json_schema_validator")
endif()

find_package(plog QUIET)
if (NOT plog_FOUND)
    message(STATUS "plog not found, fetching...")
    FetchContent_Declare(
    plog
        GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
        GIT_TAG        1.1.10
    )
    FetchContent_MakeAvailable(plog)
else()
    message(STATUS "Found plog")
endif()

find_package(Poco QUIET)
if (NOT Poco_FOUND)
    set(ENABLE_XML OFF)
    set(ENABLE_JSON OFF)
    set(ENABLE_NET OFF)
    set(ENABLE_NETSSL OFF)
    set(ENABLE_CRYPTO OFF)
    set(ENABLE_JWT OFF)
    set(ENABLE_DATA OFF)
    set(ENABLE_DATA_SQLITE OFF)
    set(ENABLE_DATA_MYSQL OFF)
    set(ENABLE_DATA_POSTGRESQL OFF)
    set(ENABLE_DATA_ODBC OFF)
    set(ENABLE_MONGODB OFF)
    set(ENABLE_REDIS OFF)
    set(ENABLE_PDF OFF)
    set(ENABLE_UTIL ON)
    set(ENABLE_ZIP OFF)
    set(ENABLE_SEVENZIP OFF)
    set(ENABLE_APACHECONNECTOR OFF)
    set(ENABLE_CPPPARSER OFF)
    set(ENABLE_ENCODINGS OFF)
    set(ENABLE_ENCODINGS_COMPILER OFF)
    set(ENABLE_PAGECOMPILER OFF)
    set(ENABLE_PAGECOMPILER_FILE2PAGE OFF)
    set(ENABLE_POCODOC OFF)
    set(ENABLE_TESTS OFF)
    set(ENABLE_SAMPLES OFF)
    set(ENABLE_LONG_RUNNING_TESTS OFF)
    set(POCO_UNBUNDLED OFF)
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
    Component/Interface.cpp
    Component/Bitset.cpp
    Observer/Plog.cpp
    Observer/IObservable.cpp
    Entity/Base.cpp
    Entity/Store.cpp
)
target_link_libraries(
    nek-core
    PUBLIC
    nlohmann_json
    nlohmann_json_schema_validator
    Poco::Foundation
    plog
)
target_include_directories(
    nek-core
    PUBLIC
    ../include
    ../../components/include
    ../../systems/include
)
target_include_directories(
    nek-core
    PRIVATE
    ${Poco_INCLUDE_DIRS}
    ${nlohmann_json_schema_validator_SOURCE_DIR}/src/nlohmann
)
target_link_directories(
    nek-core
    PUBLIC
    ${Poco_LIBRARY_DIRS}
)