macro(cocos2dx_depend)
    # confim the libs, prepare to link
    set(PLATFORM_SPECIFIC_LIBS)

    if(WINDOWS)
        list(APPEND PLATFORM_SPECIFIC_LIBS ws2_32 userenv psapi winmm Version Iphlpapi opengl32)
    elseif(ANDROID)
        list(APPEND PLATFORM_SPECIFIC_LIBS GLESv2 EGL log android OpenSLES)
    elseif(APPLE)

        include_directories(/System/Library/Frameworks)
        find_library(ICONV_LIBRARY iconv)
        find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
        find_library(FOUNDATION_LIBRARY Foundation)
        find_library(OPENAL_LIBRARY OpenAL)
        find_library(QUARTZCORE_LIBRARY QuartzCore)
        find_library(GAMECONTROLLER_LIBRARY GameController)
        find_library(SQLITE3_LIBRARY SQLite3)
        find_library(SECURITY_LIBRARY Security)
        find_library(SYSTEM_CONFIGURATION_LIBRARY SystemConfiguration REQUIRED)
        find_library(IOKIT_LIBRARY IOKit REQUIRED)
        set(COCOS_APPLE_LIBS
            ${OPENAL_LIBRARY}
            ${AUDIOTOOLBOX_LIBRARY}
            ${QUARTZCORE_LIBRARY}
            ${FOUNDATION_LIBRARY}
            ${ICONV_LIBRARY}
            ${GAMECONTROLLER_LIBRARY}
            ${SQLITE3_LIBRARY}
            ${SECURITY_LIBRARY}
            ${SYSTEM_CONFIGURATION_LIBRARY}
            ${IOKIT_LIBRARY}
            )

        if(MACOSX)
            list(APPEND PREBUILT_SPECIFIC_LIBS GLFW3)

            find_library(COCOA_LIBRARY Cocoa)
            find_library(OPENGL_LIBRARY OpenGL)
            find_library(APPLICATIONSERVICES_LIBRARY ApplicationServices)
            find_library(APPKIT_LIBRARY AppKit)
            list(APPEND PLATFORM_SPECIFIC_LIBS
                 ${COCOA_LIBRARY}
                 ${OPENGL_LIBRARY}
                 ${APPLICATIONSERVICES_LIBRARY}
                 ${COCOS_APPLE_LIBS}
                 ${APPKIT_LIBRARY}
                 )
        elseif(IOS)
            # Locate system libraries on iOS
            find_library(UIKIT_LIBRARY UIKit REQUIRED)
            find_library(OPENGLES_LIBRARY OpenGLES)
            find_library(CORE_MOTION_LIBRARY CoreMotion)
            find_library(AVKIT_LIBRARY AVKit)
            find_library(CORE_MEDIA_LIBRARY CoreMedia)
            find_library(CORE_TEXT_LIBRARY CoreText)
            find_library(CORE_GRAPHICS_LIBRARY CoreGraphics)
            find_library(AV_FOUNDATION_LIBRARY AVFoundation)
            find_library(Z_LIBRARY z)
            find_library(WEBKIT_LIBRARY WebKit)
            find_library(CFNETWORK_LIBRARY CFNetwork REQUIRED)


            list(APPEND PLATFORM_SPECIFIC_LIBS
                 ${UIKIT_LIBRARY}
                 ${OPENGLES_LIBRARY}
                 ${CORE_MOTION_LIBRARY}
                 ${AVKIT_LIBRARY}
                 ${CORE_MEDIA_LIBRARY}
                 ${CORE_TEXT_LIBRARY}
                 ${CORE_GRAPHICS_LIBRARY}
                 ${AV_FOUNDATION_LIBRARY}
                 ${Z_LIBRARY}
                 ${WEBKIT_LIBRARY}
                 ${COCOS_APPLE_LIBS}
                 ${CFNETWORK_LIBRARY}
                 )
        endif()
    endif()
endmacro()

macro(use_cocos2dx_libs_depend target)
    cocos2dx_depend()
    foreach(platform_lib ${PLATFORM_SPECIFIC_LIBS})
        target_link_libraries(${target} ${platform_lib})
        message(STATUS "SEARCH FOR LIBRARIES ${target} <- ${platform_lib}")
    endforeach()
endmacro()

