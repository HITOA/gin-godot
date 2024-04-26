cmake_minimum_required(VERSION 3.26)
project(gin-godot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if (MSVC)
    add_compile_options(/arch:AVX2)
else()
    add_compile_options(-mavx2)
endif()

add_subdirectory(3rdparty/gin)
add_subdirectory(3rdparty/godot-cpp)

add_library(gin-godot SHARED
        src/entry.cpp
        src/gingraph.hpp
        src/gingraph.cpp
        src/meshbuilder.hpp
        src/meshbuilder.cpp
)

target_link_libraries(gin-godot gin_library godot-cpp)

set(LIB_NAME "gin-godot")

if (WIN32)
    set(LIB_NAME "${LIB_NAME}.windows")
elseif (UNIX)
    set(LIB_NAME "${LIB_NAME}.linux")
endif()

if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    set(LIB_NAME "${LIB_NAME}.debug.x86_64")
else()
    set(LIB_NAME "${LIB_NAME}.release.x86_64")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${LIB_NAME}")