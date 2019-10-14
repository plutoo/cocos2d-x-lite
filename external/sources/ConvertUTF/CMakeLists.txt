cmake_minimum_required(VERSION 3.8)


project(ConvertUTF CXX)

add_library(ConvertUTF 
    ConvertUTF.h
    ConvertUTF.c
    ConvertUTFWrapper.cpp
)

target_include_directories(ConvertUTF PUBLIC ${CMAKE_CURRENT_LIST_DIR})