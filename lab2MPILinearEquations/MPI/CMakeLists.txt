cmake_minimum_required(VERSION 3.24)
project(MPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(MPI main.cpp class_mpi_matrix.cpp class_mpi_matrix.h class_mpi_options.cpp sofe_algorithm_mpi_solution.cpp sofe_algorithm_mpi_solution.h sofe_algorithm_mpi_solution.cpp sofe_algorithm_mpi_solution.cpp)
