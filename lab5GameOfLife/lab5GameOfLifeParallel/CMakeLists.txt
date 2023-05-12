cmake_minimum_required(VERSION 3.25)
project(lab5GameOfLifeParallel)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab5GameOfLifeParallel main.cpp class_cell.h constants.h class_universe.cpp class_universe.h)
