set(CC_PLATFORM_SOURCES
    platform/CCApplication.h
    platform/CCCanvasRenderingContext2D.h
    platform/CCDevice.h
    platform/CCFileUtils.h
    platform/CCGL.h
    platform/CCImage.h
    platform/CCPlatformConfig.h
    platform/CCPlatformDefine.h
    platform/CCSAXParser.h
    platform/CCStdC.h

    platform/CCFileUtils.cpp
    platform/CCImage.cpp
    platform/CCSAXParser.cpp
)

if(WINDOWS)
    list(APPEND CC_PLATFORM_SOURCES

        platform/desktop/CCGLView-desktop.h
        platform/win32/CCFileUtils-win32.h
        platform/win32/CCGL-win32.h
        platform/win32/CCPlatformDefine-win32.h
        platform/win32/CCStdC-win32.h
        platform/win32/CCUtils-win32.h
        platform/win32/compat/stdint.h
        platform/win32/inet_pton_mingw.h
    

        platform/desktop/CCGLView-desktop.cpp
        platform/win32/CCApplication-win32.cpp
        platform/win32/CCCanvasRenderingContext2D-win32.cpp
        platform/win32/CCDevice-win32.cpp
        platform/win32/CCFileUtils-win32.cpp
        platform/win32/CCStdC-win32.cpp
        platform/win32/CCUtils-win32.cpp
        platform/win32/inet_pton_mingw.cpp
    )
elseif(MACOSX)


elseif(IOS)

elseif(ANDROID)
list(APPEND CC_PLATFORM_SOURCES
    platform/android/CCDevice-android.cpp
    platform/android/CCFileUtils-android.cpp 
    platform/android/CCApplication-android.cpp
    platform/android/CCCanvasRenderingContext2D-android.cpp
    platform/android/jni/JniImp.cpp 
    platform/android/jni/JniHelper.cpp
)
endif()

