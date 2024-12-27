cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(
    nek
    VERSION 0.1.0
    LANGUAGES CXX
)

include(FetchContent)

add_subdirectory(core)
add_subdirectory(engine)
add_subdirectory(components)
add_subdirectory(systems)