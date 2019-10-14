cmake_minimum_required(VERSION 3.8)


project(clipper CXX)

add_library(clipper 
    clipper.hpp
    clipper.cpp
)

target_include_directories(clipper PUBLIC ${CMAKE_CURRENT_LIST_DIR})