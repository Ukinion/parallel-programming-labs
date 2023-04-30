cmake_minimum_required(VERSION 3.25)
project(lab5GameOfLife)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab5GameOfLife main.cpp class_universe.cpp class_universe.h class_cell.h class_game_controller.cpp class_game_controller.h constants.h)
