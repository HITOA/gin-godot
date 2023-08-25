cmake_minimum_required(VERSION 3.2)

project(gin-godot)

set(CMAKE_CXX_STANDARD 17)
set(EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

add_subdirectory(${EXTERNAL_PATH}/gin)
add_subdirectory(${EXTERNAL_PATH}/godot-cpp)

file(GLOB_RECURSE gin_godot_src "src/*.cpp" "src/*.hpp")

add_library(gin_godot SHARED ${gin_godot_src})

target_link_libraries(gin_godot gin_library godot-cpp)