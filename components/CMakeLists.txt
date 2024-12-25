set(CMAKE_CXX_FLAGS "-frtti -fPIC -no-pie")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_BUILD_TYPE Release)

add_library(
    CAlive
    SHARED
    CAlive.cpp
)
target_link_libraries(
    CAlive
    PRIVATE
    nek-core
)
set_target_properties(
    CAlive
    PROPERTIES
    OUTPUT_NAME
    "CAlive"
)

add_library(
    CPicture
    SHARED
    CPicture.cpp
)
target_link_libraries(
    CPicture
    PRIVATE
    nek-core
)
set_target_properties(
    CPicture
    PROPERTIES
    OUTPUT_NAME
    "CPicture"
)