include(${CMAKE_CURRENT_LIST_DIR}/sources/CMakeLists.txt)

set(CC_EXTERNAL_LIBS
    # dbghelp
    freetype
    glew32
    glfw3
    iconv
    crypto
    curl
    jpeg
    mpg123
    ogg
    png
    ssl
    tiff
    uv
    vorbis
    vorbisfile
    webp
    zlib
    #unzip
    # nspr4
    OpenAL32
    sqlite3
    v8
    websockets
)

if(WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/win32/CMakeLists.txt)
elseif(ANDROID)
    include(${CMAKE_CURRENT_LIST_DIR}/android/CMakeLists.txt)
elseif(APPLE)
    if(MACOSX)
        include(${CMAKE_CURRENT_LIST_DIR}/mac/CMakeLists.txt)
    elseif(IOS)
        include(${CMAKE_CURRENT_LIST_DIR}/ios/CMakeLists.txt)
    endif()
endif()

