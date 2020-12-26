cmake_minimum_required(VERSION 3.19)
project(task3_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp abstract_graph.h ListGraph.h ListGraph.cpp MatrixGraph.cpp MatrixGraph.h SetGraph.cpp SetGraph.h)