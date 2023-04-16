cmake_minimum_required(VERSION 3.24)
project(noMPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(noMPI main.cpp "class_matrix.cpp" "class_matrix.h" sofe_algorithm_solution.h sofe_algorithm_solution.cpp)
