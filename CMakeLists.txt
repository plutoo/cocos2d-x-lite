cmake_minimum_required(VERSION 3.8)


project(cocos2d CXX)

option(USE_SPINE "Enable SpineAPI" ON)
option(USE_DRAGONBONES "Enable DragonBonesAPI" ON)
option(CC_V8_DEBUGGER "Enable V8 inspector" ON)

add_subdirectory(cocos engine)

