
set(COCOSLIB cocos2d)


option(USE_SPINE "Enable SpineAPI" ON)
option(USE_DRAGONBONES "Enable DragonBonesAPI" ON)


include(audio/CMakeLists.txt)
include(platform/CMakeLists.txt)
include(editor-support/CMakeLists.txt)
include(network/CMakeLists.txt)
include(scripting/CMakeLists.txt)
include(renderer/CMakeLists.txt)

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
    

    ui/edit-box/EditBox-win32.cpp
    storage/local-storage/LocalStorage.cpp
)


set(COCOS_SOUCECODE_SOURCES 
    ${CC_BASE_HEADERS} 
    ${CC_BASE_SOURCES}
    ${CC_AUDIO_SOURCES}
    ${CC_EDITORSUPPORT_SOURCES}
    ${CC_NETWORK_SOURCES}
    ${CC_SCRIPTING_SOURCES}
    ${CC_RENDERER_SOURCES}
    ${CC_PLATFORM_SOURCES}
    ${CC_EXTERNAL_SROUCES}
)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include/v8
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include/zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/win32/include/uv
)

use_cocos2dx_compile_define(${COCOSLIB})
target_compile_definitions(${COCOSLIB} PUBLIC 
    $<$<NOT:$<BOOL:${USE_SPINE}>>:USE_SPINE=0>
    $<$<NOT:$<BOOL:${USE_DRAGONBONES}>>:USE_DRAGONBONES=0>
)



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