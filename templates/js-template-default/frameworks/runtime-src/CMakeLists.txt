cmake_minimum_required(VERSION 3.8)

set(APP_NAME ExampleCollection)


set(COCOS2DX_LITE_ROOT "C:/Github/cocos2d-x-lite")
set(CMAKE_MODULE_PATH ${COCOS2DX_LITE_ROOT}/cmake/Modules)

project(${APP_NAME} CXX)

include(CocosBuildSet)

add_subdirectory(${COCOS2DX_LITE_ROOT}/cocos native_engine)

set(GAME_SOURCES
    Classes/AppDelegate.cpp
    Classes/jsb_module_register.cpp
)
set(GAME_HEADERS
    Classes/AppDelegate.h
)


set(GAME_RES_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../)

set(GAME_RES_FOLDERS 
    ${GAME_RES_ROOT}/jsb-adapter
    ${GAME_RES_ROOT}/res
    ${GAME_RES_ROOT}/src
    ${GAME_RES_ROOT}/main.js
    ${GAME_RES_ROOT}/project.json
)

if(APPLE OR VS)
    cocos_mark_multi_resources(cc_common_res RES_TO "Resources" FOLDERS ${GAME_RES_FOLDERS})
endif()


if(WINDOWS)
    list(APPEND GAME_SOURCES
        proj.win32/main.cpp
        proj.win32/game.rc
        ${cc_common_res}
    )
    list(APPEND GAME_HEADERS
        proj.win32/main.h
        proj.win32/resource.h
    )
endif()

set(ALL_SOURCE_CODE 
    ${GAME_SOURCES}
    ${GAME_HEADERS}
)

if(NOT ANDROID)
    add_executable(${APP_NAME} ${ALL_SOURCE_CODE})
else()
    add_library(${APP_NAME} SHARED ${ALL_SOURCE_CODE})
endif()



set_property(TARGET ${APP_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
    PRIVATE Classes/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})


if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon-${APP_NAME}")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    # "too large PDB" error often occurs in cpp-tests when using default "/Zi"
    target_compile_options(${APP_NAME} PRIVATE /Z7)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} DIRFILES ${GAME_RES_FOLDERS})
endif()

# if(WINDOWS)
#     # precompiled header. Compilation time speedup ~4x.
#     target_sources(${APP_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Classes/precheader.cpp")
#     set_target_properties(${APP_NAME} PROPERTIES COMPILE_FLAGS "/Yuprecheader.h /FIprecheader.h")
#     set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/Classes/precheader.cpp" PROPERTIES COMPILE_FLAGS "/Ycprecheader.h")
# endif()
