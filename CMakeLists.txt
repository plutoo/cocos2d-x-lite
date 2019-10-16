cmake_minimum_required(VERSION 3.8)

project(cocos2d CXX)

enable_language(ASM C)


option(USE_SPINE "Enable SpineAPI" ON)
option(USE_DRAGONBONES "Enable DragonBonesAPI" ON)
option(USE_V8_DEBUGGER "Enable V8 inspector" ON)

set(USE_GFX_RENDERER OFF)
set(USE_VIDEO ON)
set(USE_WEBVIEW ON)
set(USE_AUDIO ON)
set(USE_SOCKET ON)
set(USE_TIFF ON)
set(USE_MIDDLEWARE ON) #USE_SPINE or USE_DRAGONBONES

if(NOT USE_MIDDLEWARE)
    set(USE_SPINE OFF)
    set(USE_DRAGONBONES OFF)
endif()

add_subdirectory(cocos cocos2d)

