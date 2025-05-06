cmake_minimum_required(VERSION 3.28)
project(impulse_2025_cpp)

set(CMAKE_CXX_STANDARD 20)

add_library(chronoio src/chronoio.cpp src/chronoio.h)
add_library(action src/action.cpp src/action.h)
add_library(club src/club.cpp src/club.h)

add_executable(task src/main.cpp)

target_link_libraries(task chronoio action club)
