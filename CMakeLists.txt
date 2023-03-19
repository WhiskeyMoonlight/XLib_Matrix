cmake_minimum_required(VERSION 3.16)
project(RK1)

set(CMAKE_CXX_STANDARD 14)

add_executable(RK1 DetGraphics.h)

add_executable(RK2 DetGraphics.h main.cpp    functions.cpp)
