

set(CC_NETWORK_SOURCES
    network/CCDownloader-curl.h
    network/CCDownloader.h
    network/CCIDownloaderImpl.h
    network/HttpClient.h
    network/HttpCookie.h
    network/HttpRequest.h
    network/HttpResponse.h
    network/SocketIO.h
    network/Uri.h
    network/WebSocket.h
    network/CCDownloader.cpp
    network/HttpCookie.cpp
    network/SocketIO.cpp
    network/Uri.cpp
)


if(USE_SOCKET)
    list(APPEND CC_NETWORK_SOURCES
        network/SocketIO.cpp
    )
endif()

if(WINDOWS)
    list(APPEND CC_NETWORK_SOURCES 
        network/HttpClient.cpp
        network/CCDownloader-curl.cpp
        network/WebSocket-libwebsockets.cpp
    )
    if(USE_SOCKET)
        list(APPEND CC_NETWORK_SOURCES
            network/WebSocket-libwebsockets.cpp
        )
    endif()
elseif(ANDROID)
    list(APPEND CC_NETWORK_SOURCES 
        network/CCDownloader-android.cpp
        network/HttpClient-android.cpp
    )
    if(USE_SOCKET)
        list(APPEND CC_NETWORK_SOURCES
            network/WebSocket-libwebsockets.cpp
        )
    endif()
elseif(APPLE)
    list(APPEND CC_NETWORK_SOURCES 
        network/CCDownloaderImpl-apple.mm
        network/HttpAsynConnection-apple.m
        network/HttpClient-apple.mm
    )
    if(USE_SOCKET)
        list(APPEND CC_NETWORK_SOURCES
            network/WebSocket-apple.mm
        )
        set_source_files_properties(network/WebSocket-apple.mm PROPERTIES
            COMPILE_FLAGS -fobjc-arc
        )
    endif()
endif()
