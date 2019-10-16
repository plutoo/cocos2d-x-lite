cmake_minimum_required(VERSION 3.8)

project(cocos2d CXX)

enable_language(ASM C)

if(NOT USE_MIDDLEWARE)
    set(USE_SPINE OFF)
    set(USE_DRAGONBONES OFF)
endif()

if(NOT USE_SE_JSC AND NOT USE_SE_SM AND NOT USE_SE_V8)
    # use default setting
    set(USE_SE_AUTO ON)
    
    if(IOS) 
        set(USE_SE_JSC ON)
    else()
        set(USE_SE_V8 ON)
    endif()
else()
    set(USE_SE_AUTO OFF)
    
    set(enabled_engine_cnt "")
    if(USE_SE_JSC)
        set(enabled_engine_cnt "${enabled_engine_cnt}x")
    endif()
    
    if(USE_SE_SM)
        set(enabled_engine_cnt "${enabled_engine_cnt}x")
    endif()
    
    if(USE_SE_V8)
        set(enabled_engine_cnt "${enabled_engine_cnt}x")
    endif()

    if(NOT ${enabled_engine_cnt} STREQUAL "x") 
        string(LENGTH ${enabled_engine_cnt} enabled_engine_cnt_len)
        message(FATAL_ERROR "Only one script engine can be enabled on each platform! Got ${enabled_engine_cnt_len}!")
    endif()
endif()

set(ENGINE_NAME)

if(USE_SE_JSC) 
    set(ENGINE_NAME "JavaScriptCore")
endif()

if(USE_SE_SM) 
    set(ENGINE_NAME "SpiderMonkey")
endif()

if(USE_SE_V8) 
    set(ENGINE_NAME "V8")
endif()

message(STATUS "Using script engine `${ENGINE_NAME}` on ${CMAKE_SYSTEM_NAME}")

add_subdirectory(cocos cocos2d)

