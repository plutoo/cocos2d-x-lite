cmake_minimum_required(VERSION 3.8)

set(APP_NAME "HelloJavascript")
project(${APP_NAME} CXX)

## Set engine path
set(COCOS2DX_LITE_ROOT "C:/Github/cocos2d-x-lite")
if(NOT EXISTS "${COCOS2DX_LITE_ROOT}" AND EXISTS "$ENV{COCOS_X_ROOT}")
    set(COCOS2DX_LITE_ROOT $ENV{COCOS_X_ROOT})
endif()

if(NOT EXISTS "${COCOS2DX_LITE_ROOT}")
    message(FATAL_ERROR "COCOS2DX_LITE_ROOT invalidate : '${COCOS2DX_LITE_ROOT}' , '$ENV{COCOS_X_ROOT}'")
endif()

set(CMAKE_MODULE_PATH ${COCOS2DX_LITE_ROOT}/cmake/Modules)
include(CocosBuildSet)


## Engine configuration
set(USE_MIDDLEWARE      ON)     #Use Spine or Dragonbones
set(USE_SPINE           ON)     #Enable SpineAPI
set(USE_DRAGONBONES     ON)     #Enable DragonBonesAPI
set(USE_V8_DEBUGGER     ON)     #Enable V8 Inspector
set(USE_GFX_RENDERER    OFF)    #Enable GFX Renderer
set(USE_VIDEO           ON)     #Enable VideoPlayer
set(USE_WEB_VIEW        ON)     #Enable WebView
set(USE_AUDIO           ON)     #Enable AudioEngine
set(USE_SOCKET          ON)     #Enable WebSocket & SocketIO
set(USE_TIFF            ON)     #Enable TIFF format support
set(USE_CXX_STANDARD    11)


set(OPT_COPY_RESOURCE_EVERYTIME ON)

## Only one engine can be enable on each platform
## Use default engine setting by leave them unset
# set(USE_SE_V8           OFF) 
# set(USE_SE_SM           OFF)
# set(USE_SE_JSC          OFF)

add_subdirectory(${COCOS2DX_LITE_ROOT} engine)

## setup target name

if(ANDROID)
    set(LIB_NAME cocos2djs)
elseif(MACOSX)
    set(LIB_NAME ${APP_NAME}-desktop)
elseif(IOS)
    set(LIB_NAME ${APP_NAME}-mobile)
else()
    set(LIB_NAME ${APP_NAME})
endif()

## setup resources & source files

set(GAME_SOURCES
    Classes/AppDelegate.cpp
    Classes/jsb_module_register.cpp
)

set(GAME_HEADERS
    Classes/AppDelegate.h
)

## point to path `buid/jsb-link/`
set(GAME_RES_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../)

set(GAME_RES_FILES
    ${GAME_RES_ROOT}/main.js
    ${GAME_RES_ROOT}/project.json
)

set(GAME_RES_FOLDERS 
    ${GAME_RES_ROOT}/jsb-adapter
    ${GAME_RES_ROOT}/subpackages
    ${GAME_RES_ROOT}/res
    ${GAME_RES_ROOT}/src
)
set(GAME_RES_ALL
    ${GAME_RES_FILES}
    ${GAME_RES_FOLDERS}
)

if(APPLE OR VS)
    cocos_mark_multi_resources(cc_common_res RES_TO "Resources" FILES ${GAME_RES_FILES} FOLDERS ${GAME_RES_FOLDERS})
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
elseif(ANDROID)
    list(APPEND GAME_SOURCES
        proj.android-studio/app/jni/hellojavascript/main.cpp
    )
elseif(MACOSX)
    list(APPEND GAME_SOURCES
        proj.ios_mac/mac/Icon.icns
        proj.ios_mac/mac/Prefix.pch
        proj.ios_mac/mac/main.cpp
        proj.ios_mac/mac/UserConfigMac.debug.xcconfig
        proj.ios_mac/mac/UserConfigMac.release.xcconfig
    )

    list(APPEND GAME_SOURCES 
        ${cc_common_res}
    )
elseif(IOS)

    list(APPEND GAME_SOURCES 
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/SDKWrapper.h
            proj.ios_mac/ios/SDKWrapper.m
            proj.ios_mac/ios/RootViewController.h
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/service/SDKDelegate.h
            ${cc_common_res}
        #    proj.ios_mac/ios/Base.lproj
            proj.ios_mac/ios/LaunchScreenBackground.png
        #    proj.ios_mac/ios/zh-Hans.lproj
            proj.ios_mac/ios/UserConfigIOS.debug.xcconfig
            proj.ios_mac/ios/UserConfigIOS.release.xcconfig
            proj.ios_mac/ios/Images.xcassets
         #   proj.ios_mac/ios/Base.lproj/Localizable.strings
            proj.ios_mac/ios/Base.lproj/LaunchScreen.storyboard
        )

    set_source_files_properties(proj.ios_mac/ios/LaunchScreenBackground.png
        PROPERTIES MACOSX_PACKAGE_LOCATION ios
        HEADER_FILE_ONLY 1
    )
endif()

set(ALL_SOURCE_CODE 
    ${GAME_SOURCES}
    ${GAME_HEADERS}
)

## setup target & compile options

if(NOT ANDROID)
    add_executable(${LIB_NAME} ${ALL_SOURCE_CODE})
else()
    add_library(${LIB_NAME} SHARED ${ALL_SOURCE_CODE})
endif()


set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD ${USE_CXX_STANDARD})

target_link_libraries(${LIB_NAME} cocos2d)
target_include_directories(${LIB_NAME}
    PRIVATE Classes/
)

# mark app resources
setup_cocos_app_config(${LIB_NAME})


if(APPLE)
    
    set_target_properties(${LIB_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${LIB_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )  
    elseif(IOS)
        target_include_directories(${LIB_NAME} PRIVATE 
            ${CMAKE_CURRENT_LIST_DIR}/proj.ios_mac/ios
            ${CMAKE_CURRENT_LIST_DIR}/proj.ios_mac/ios/service
            )
        set_target_properties(${LIB_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist"
                              )  
        set_xcode_property(${LIB_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${LIB_NAME} ASSETCATALOG_COMPILER_LAUNCHIMAGE_NAME "LaunchScreen")
        set_xcode_property(${LIB_NAME} ASSETCATALOG_COMPILER_LAUNCHSTORYBOARD_NAE "LaunchScreen")
        set_xcode_property(${LIB_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${LIB_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
        cocos_config_app_xcode_property(${LIB_NAME})
    endif()
    
elseif(WINDOWS)

    if(MSVC)
        set(VS_STARTUP_PROJECT ${LIB_NAME})
        # "too large PDB" error often occurs when using default "/Zi"
        target_compile_options(${LIB_NAME} PRIVATE /Z7)
        target_compile_definitions(${LIB_NAME} PRIVATE
            _USRLIBSIMSTATIC
            STRICT
        )
    endif()

    get_target_property(APP_RES_DIR ${LIB_NAME} RUNTIME_OUTPUT_DIRECTORY)
    target_link_directories(${LIB_NAME} PRIVATE ${APP_RES_DIR})
    if(OPT_COPY_RESOURCE_EVERYTIME AND APP_RES_DIR AND NOT OUTPUT_DIRECTORY)
        ## override runtime output directory
        set_target_properties(${LIB_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_DEBUG ${APP_RES_DIR}/Debug
                RUNTIME_OUTPUT_DIRECTORY_RELEASE ${APP_RES_DIR}/Release
            )
        cocos_create_copy_res_target(${LIB_NAME} COPY_TO ${APP_RES_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release> DIRFILES ${GAME_RES_ALL})
    else()
        cocos_copy_target_res(${LIB_NAME} COPY_TO ${APP_RES_DIR} DIRFILES ${GAME_RES_ALL})
    endif()
    
    # copy dll files
    file(GLOB CC_EXT_WINDLL ${COCOS2DX_LITE_ROOT}/external/win32/libs/*.dll)
    file(GLOB CC_EXT_WINDLL_DBG ${COCOS2DX_LITE_ROOT}/external/win32/libs/Debug/*.dll)
    add_custom_command(TARGET ${LIB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CC_EXT_WINDLL} ${CC_EXT_WINDLL_DBG} $<TARGET_FILE_DIR:${LIB_NAME}>
    )

endif()

