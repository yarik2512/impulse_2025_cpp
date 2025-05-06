cmake_minimum_required(VERSION 3.28)
project(yadro_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(yadro_cpp
        src/chronoio.cpp
        src/chronoio.h
        src/action.cpp
        src/action.h
        src/club.cpp
        src/club.h
        src/main.cpp)
