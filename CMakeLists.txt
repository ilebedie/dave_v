cmake_minimum_required(VERSION 3.10)

project(dave)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(dave ${SDL2_INCLUDE_DIRS})

add_executable(tiles tiles.cpp)
add_executable(level level.cpp)
add_executable(dave dave.cpp)
target_link_libraries(tiles ${SDL2_LIBRARIES})
target_link_libraries(level ${SDL2_LIBRARIES})
target_link_libraries(dave ${SDL2_LIBRARIES})

