
set(COCOSLIB cocos2d)

include(audio/CMakeLists.txt)
include(platform/CMakeLists.txt)
include(editor-support/CMakeLists.txt)
include(network/CMakeLists.txt)
include(scripting/CMakeLists.txt)
include(renderer/CMakeLists.txt)
include(../extensions/CMakeLists.txt)
include(../external/CMakeLists.txt)

set(CC_BASE_HEADERS 
    cocos2d.h
    base/base64.h
    base/CCAutoreleasePool.h
    base/ccCArray.h
    base/ccConfig.h
    base/CCConfiguration.h
    base/CCData.h
    base/CCGLUtils.h
    base/CCLog.h
    base/ccMacros.h
    base/CCMap.h
    base/ccRandom.h
    base/CCRef.h
    base/CCRefPtr.h
    base/CCRenderTexture.h
    base/CCScheduler.h
    base/CCThreadPool.h
    base/ccTypes.h
    base/ccUTF8.h
    base/ccUtils.h
    base/CCValue.h
    base/CCVector.h
    base/etc1.h
    base/pvr.h
    base/TGAlib.h
    base/uthash.h
    base/utlist.h
    base/ZipUtils.h

    math/CCGeometry.h
    math/CCMath.h
    math/CCMathBase.h
    math/CCVertex.h
    math/Mat4.h
    math/MathUtil.h
    math/Quaternion.h
    math/Vec2.h
    math/Vec3.h
    math/Vec4.h

    ui/edit-box/EditBox.h
    storage/local-storage/LocalStorage.h
)


set(CC_BASE_SOURCES
    cocos2d.cpp
    base/base64.cpp
    base/CCAutoreleasePool.cpp
    base/ccCArray.cpp
    base/CCConfiguration.cpp
    base/CCData.cpp
    base/CCGLUtils.cpp
    base/CCLog.cpp
    base/ccRandom.cpp
    base/CCRef.cpp
    base/CCRenderTexture.cpp
    base/CCScheduler.cpp
    base/CCThreadPool.cpp
    base/ccTypes.cpp
    base/ccUTF8.cpp
    base/ccUtils.cpp
    base/CCValue.cpp
    base/csscolorparser.cpp
    base/etc1.cpp
    base/pvr.cpp
    base/TGAlib.cpp
    base/ZipUtils.cpp
    
    math/CCGeometry.cpp
    math/CCVertex.cpp
    math/Mat4.cpp
    math/MathUtil.cpp
    math/Quaternion.cpp
    math/Vec2.cpp
    math/Vec3.cpp
    math/Vec4.cpp
    
)

if(WINDOWS)
    list(APPEND CC_BASE_SOURCES
        ui/edit-box/EditBox-win32.cpp    
        storage/local-storage/LocalStorage.cpp
    )
elseif(ANDROID)
    list(APPEND CC_BASE_SOURCES
        ui/edit-box/EditBox-android.cpp
        storage/local-storage/LocalStorage-android.cpp
    )
    if(USE_VIDEO)
        list(APPEND CC_BASE_SOURCES
            ui/videoplayer/VideoPlayer-android.cpp
        )
    endif()
    if(USE_WEB_VIEW)
        list(APPEND CC_BASE_SOURCES
            ui/webview/WebViewImpl-android.cpp
        )
    endif()
elseif(MACOSX)
    list(APPEND CC_BASE_SOURCES
        ui/edit-box/EditBox-mac.mm
        storage/local-storage/LocalStorage.cpp
    )
elseif(IOS)
    list(APPEND CC_BASE_SOURCES
        ui/edit-box/EditBox-ios.mm
        storage/local-storage/LocalStorage.cpp
    )
    if(USE_VIDEO)
        list(APPEND CC_BASE_SOURCES
            ui/videoplayer/VideoPlayer-ios.mm
        )
    endif()
    if(USE_WEB_VIEW)
        list(APPEND CC_BASE_SOURCES
            ui/webview/WebViewImpl-ios.mm
        )
    endif()
endif()




set(COCOS_SOUCECODE_SOURCES 
    ${CC_BASE_HEADERS} 
    ${CC_BASE_SOURCES}
    ${CC_NETWORK_SOURCES}
    ${CC_SCRIPTING_SOURCES}
    ${CC_RENDERER_SOURCES}
    ${CC_PLATFORM_SOURCES}
    ${CC_EXTERNAL_SROUCES}
    ${CC_EXTENSIONS_SOURCES}
)

if(USE_AUDIO)
    list(APPEND COCOS_SOUCECODE_SOURCES
        ${CC_AUDIO_SOURCES}
    )
endif()

if(USE_MIDDLEWARE)
    list(APPEND COCOS_SOUCECODE_SOURCES
        ${CC_EDITORSUPPORT_SOURCES}
    )
endif()

add_library(${COCOSLIB} ${COCOS_SOUCECODE_SOURCES})

set_property(TARGET ${COCOSLIB} PROPERTY CXX_STANDARD 11)

target_include_directories(${COCOSLIB} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../extensions
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/editor-support
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/sources
    ${CC_EXTERNAL_INCLUDES}
)

if(WINDOWS)

target_include_directories(${COCOSLIB} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include/zlib
)

elseif(ANDROID)
    target_include_directories(${COCOSLIB} PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/android/${ANDROID_ABI}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/android/${ANDROID_ABI}/include/zlib
    )
    if(ANDROID_NDK)
        target_include_directories(${COCOSLIB} PUBLIC 
            ${ANDROID_NDK}/sources/android/cpufeatures
        )
    else()
        target_include_directories(${COCOSLIB} PUBLIC 
            ${CMAKE_ANDROID_NDK}/sources/android/cpufeatures
        )
    endif()
elseif(MACOSX)
    target_include_directories(${COCOSLIB} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/mac/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/sources/
        ##TODO mac depends ios ?
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/ios
    )
    
    # target_link_options(${COCOSLIB} PUBLIC
    #     -llibsql3
    # )
elseif(IOS)

endif()

use_cocos2dx_compile_define(${COCOSLIB})

#TODO: document
target_compile_definitions(${COCOSLIB} PUBLIC 
    $<$<NOT:$<BOOL:${USE_VIDEO}>>:USE_VIDEO=0>
    $<$<NOT:$<BOOL:${USE_WEB_VIEW}>>:USE_WEB_VIEW=0>
    $<$<NOT:$<BOOL:${USE_AUDIO}>>:USE_AUDIO=0>
    $<$<NOT:$<BOOL:${USE_SOCKET}>>:USE_SOCKET=0>
    $<$<NOT:$<BOOL:${USE_MIDDLEWARE}>>:USE_MIDDLEWARE=0>
    $<$<NOT:$<BOOL:${USE_SPINE}>>:USE_SPINE=0>
    $<$<NOT:$<BOOL:${USE_DRAGONBONES}>>:USE_DRAGONBONES=0>
    $<$<NOT:$<BOOL:${USE_TIFF}>>:CC_USE_TIFF=0>
    $<$<BOOL:${USE_GFX_RENDERER}>:USE_GFX_RENDERER=1>
    $<$<NOT:$<BOOL:${USE_V8_DEBUGGER}>>:USE_V8_DEBUGGER=0>
)


if(NOT USE_SE_AUTO)   
    if(USE_SE_SM) 
        target_compile_definitions(${COCOSLIB} PUBLIC
            SCRIPT_ENGINE_TYPE=1
        )
        # if(WINDOWS)
        #     target_compile_definitions(${COCOSLIB} PUBLIC
        #         _HAS_ITERATOR_DEBUGGING=0
        #         _ITERATOR_DEBUG_LEVEL=0
        #     )
        # endif()
    endif()
    if(USE_SE_V8) 
        target_compile_definitions(${COCOSLIB} PUBLIC
            SCRIPT_ENGINE_TYPE=2
        )
    endif()
    if(USE_SE_JSC) 
        target_compile_definitions(${COCOSLIB} PUBLIC
            SCRIPT_ENGINE_TYPE=3
        )
    endif()
endif()

if(USE_SE_V8) 


endif()


cocos_mark_code_files(${COCOSLIB})

use_cocos2dx_libs_depend(${COCOSLIB})
target_link_libraries(${COCOSLIB} 
    ${CC_EXTERNAL_LIBS}
)


if(MSVC)
    set_property(TARGET ${COCOSLIB} PROPERTY LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    target_compile_options(${COCOSLIB}
        PUBLIC /MP
    )
    target_link_directories(${COCOSLIB} PRIVATE ../external/win32/libs)
endif()