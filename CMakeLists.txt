cmake_minimum_required(VERSION 3.30)
project(segment_tree)

set(CMAKE_CXX_STANDARD 20)

add_executable(segment_tree main.cpp
        src/segment_tree.cpp
        src/segment_tree.h
        src/node.cpp
        src/node.h)
