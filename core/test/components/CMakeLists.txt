set(CMAKE_CXX_FLAGS "-frtti -fPIC -no-pie")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_BUILD_TYPE Release)

add_library(
    test-CAlive
    SHARED
    CAlive.cpp
)
target_include_directories(
    test-CAlive
    PRIVATE
    ../../include
)
set_target_properties(
    test-CAlive
    PROPERTIES
    OUTPUT_NAME
    "CAlive"
)

add_library(
    test-CPicture
    SHARED
    CPicture.cpp
)
target_include_directories(
    test-CPicture
    PRIVATE
    ../../include
)
set_target_properties(
    test-CPicture
    PROPERTIES
    OUTPUT_NAME
    "CPicture"
)