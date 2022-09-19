cmake_minimum_required(VERSION 3.10)

project(dave)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
include_directories(dave ${SDL2_INCLUDE_DIRS})

file(GLOB pc21_source "./src/**/*.cpp")
add_executable(dave_v src/dave_v.cpp ${pc21_source})
target_link_libraries(dave_v ${SDL2_LIBRARIES})

