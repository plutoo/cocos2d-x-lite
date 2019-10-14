

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


if(WINDOWS)
    list(APPEND CC_NETWORK_SOURCES 
        network/HttpClient.cpp
        network/CCDownloader-curl.cpp
        network/WebSocket-libwebsockets.cpp
    )
elseif(ANDROID)
    list(APPEND CC_NETWORK_SOURCES 
        network/CCDownloader-android.cpp
        network/WebSocket-libwebsockets.cpp
        network/HttpClient-android.cpp
    )
elseif(APPLE)
    list(APPEND CC_NETWORK_SOURCES 
        network/CCDownloaderImpl-apple.mm
        network/HttpAsynConnection-apple.mm
        network/HttpClient-apple.mm
        network/WebSocket-apple.mm
    )
endif()
