cmake_minimum_required(VERSION 3.12)
project(lab_1 C)

set(CMAKE_C_STANDARD 99)
include_directories('/usr/include/gsl')

add_executable(lab_1 main.c)
find_package(GSL REQUIRED)
target_link_libraries(lab_1 GSL::gsl GSL::gslcblas m)

