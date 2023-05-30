cmake_minimum_required(VERSION 3.25)
project(MPI)

set(CMAKE_CXX_STANDARD 17)

add_executable(MPI src/main.cpp src/constants.h src)
