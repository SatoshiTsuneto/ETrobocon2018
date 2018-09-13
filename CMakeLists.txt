cmake_minimum_required(VERSION 3.10)

project(ETrobocon2018)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        ETR2018
        app.cpp
        balancer.c
        balancer_param.c
        ./app/
        ./unit/
        ../../common/ev3api/src/
        ../../common/library/libcpp-ev3/src/
)

include_directories(
        ./
        ./app/
        ./unit/
        ../../common/ev3api/include/
        ../../common/library/libcpp-ev3/include/
        ../../../base-workspace/OBJ/
        ../../../../include/
)
