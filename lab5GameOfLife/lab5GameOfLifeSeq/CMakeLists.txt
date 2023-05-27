cmake_minimum_required(VERSION 3.25)
project(lab5GameOfLifeSeq)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(lab5GameOfLifeSeq main.cpp class_universe.cpp class_universe.h class_cell.h class_game_of_life.cpp class_game_of_life.h constants.h class_game_controller.h)
