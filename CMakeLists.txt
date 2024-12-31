cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(
    nek
    VERSION 0.1.1
    LANGUAGES CXX
)

include(FetchContent)

option(NEK_ENABLE_GAME "enable building game components and systems" ON)

add_subdirectory(core)
add_subdirectory(engine)

if(NEK_ENABLE_GAME)
    add_subdirectory(components)
    add_subdirectory(systems)
endif()