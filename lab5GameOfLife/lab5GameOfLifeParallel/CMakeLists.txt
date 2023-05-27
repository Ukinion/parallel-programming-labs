cmake_minimum_required(VERSION 3.25)
project(lab5GameOfLifeParallel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3")

add_executable(lab5GameOfLifeParallel main.cpp class_cell.h constants.h class_universe.cpp class_universe.h class_game_of_life.cpp class_game_of_life.h class_game_of_life.cpp class_game_controller.h class_game_data_scheduler.h)
