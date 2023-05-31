cmake_minimum_required(VERSION 3.25)
project(MPI)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        MPI src/main.cpp src/game-object/game_field.cpp
        src/game-object/game_field.h src/constants.h
        src/exception/game_exception.cpp src/exception/game_exception.h
        src/exception/invalid_option_exception.cpp src/exception/invalid_option_exception.h
        src/exception/out_of_exception.cpp src/exception/out_of_exception.h
        src/game-controller/launch_game_controller.cpp src/game-controller/launch_game_controller.h
        src/game-model/game_of_life.cpp src/game-model/game_of_life.h src/exception/not_comparable_exception.cpp
        src/exception/not_comparable_exception.h
)