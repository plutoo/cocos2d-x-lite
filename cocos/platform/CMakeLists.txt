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

elseif(APPLE)

    list(APPEND CC_PLATFORM_SOURCES
        platform/apple/CCFileUtils-apple.h
        platform/apple/CCDevice-apple.mm
        platform/apple/CCFileUtils-apple.mm
        platform/apple/CCCanvasRenderingContext2D-apple.mm
        platform/apple/CCDevice-apple.h
    )

    if(MACOSX) 

        list(APPEND CC_PLATFORM_SOURCES
            platform/mac/cocos2d-prefix.pch
            platform/mac/CCPlatformDefine-mac.h
            platform/mac/CCDevice-mac.mm
            platform/mac/CCApplication-mac.mm
            platform/mac/CCGL-mac.h
            platform/desktop/CCGLView-desktop.cpp
            platform/mac/CCModuleConfigMac.release.xcconfig
            platform/mac/CCModuleConfigMac.debug.xcconfig
            ## require code from ios ?
            platform/ios/CCReachability.cpp
        )
    elseif(IOS)
        list(APPEND CC_PLATFORM_SOURCES
            platform/ios/CCGL-ios.h
            platform/ios/CCReachability.h
            platform/ios/CCApplication-ios.mm
            platform/ios/cocos2d-prefix.pch
            platform/ios/CCEAGLView-ios.mm
            platform/ios/OpenGL_Internal-ios.h
            platform/ios/CCDevice-ios.mm
            platform/ios/CCReachability.cpp
            platform/ios/CCPlatformDefine-ios.h
            platform/ios/CCEAGLView-ios.h
            platform/ios/CCImage-ios.mm
            platform/ios/CCModuleConfigIOS.release.xcconfig
            platform/ios/CCModuleConfigIOS.debug.xcconfig
        )
    endif()

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

