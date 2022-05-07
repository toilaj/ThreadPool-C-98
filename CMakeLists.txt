cmake_minimum_required(VERSION 3.22)
project(ThreadPool_C_98)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ThreadPool_C_98
        main.cpp
        ThreadPool.cpp
        ThreadPool.h)
