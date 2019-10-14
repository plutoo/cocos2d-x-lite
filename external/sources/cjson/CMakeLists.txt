cmake_minimum_required(VERSION 3.8)


project(cjson C)

add_library(cjson 
    fpconv.h
    lua_cjson.h
    strbuf.h
    lua_cjson.c
    fpconv.c
    strbuf.c
)

target_include_directories(cjson PUBLIC ${CMAKE_CURRENT_LIST_DIR})