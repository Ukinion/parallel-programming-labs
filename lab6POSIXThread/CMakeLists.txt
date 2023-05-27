cmake_minimum_required(VERSION 3.25)
project(lab6POSIXThread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")

add_executable(lab6POSIXThread main.cpp constants.h global_storage.cpp global_storage.h pthread_model.cpp pthread_model.h)
