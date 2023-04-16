cmake_minimum_required(VERSION 3.25)
project(GenerateEquation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(GenerateEquation main.cpp matrix_generation.h matrix_generation.cpp)
