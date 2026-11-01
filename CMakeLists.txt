cmake_minimum_required(VERSION 3.16)

project(A5-DNA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(A5-DNA
    main.cpp
    dynamicchararray.cpp
    dynamicchararray.h
)