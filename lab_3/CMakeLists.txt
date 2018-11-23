cmake_minimum_required(VERSION 3.12)
project(lab_3 C)

set(CMAKE_C_STANDARD 99)
include_directories('/usr/include/gsl')
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")
add_executable(lab_3 main.c)

find_package(GSL REQUIRED)
target_link_libraries(lab_3 GSL::gsl GSL::gslcblas m)
