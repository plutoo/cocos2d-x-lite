


set(CC_EXTERNAL_SROUCES
  ${CMAKE_CURRENT_LIST_DIR}/tinyxml2/tinyxml2.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tinyxml2/tinyxml2.h

  ${CMAKE_CURRENT_LIST_DIR}/xxtea/xxtea.cpp
  ${CMAKE_CURRENT_LIST_DIR}/xxtea/xxtea.h

  ${CMAKE_CURRENT_LIST_DIR}/unzip/crypt.h
  ${CMAKE_CURRENT_LIST_DIR}/unzip/ioapi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/unzip/ioapi.h
  ${CMAKE_CURRENT_LIST_DIR}/unzip/ioapi_mem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/unzip/ioapi_mem.h
  ${CMAKE_CURRENT_LIST_DIR}/unzip/unzip.cpp
  ${CMAKE_CURRENT_LIST_DIR}/unzip/unzip.h
)

if(ANDROID)

  include(${CMAKE_CURRENT_LIST_DIR}/pvmp3dec/CMakeLists.txt)
  include(${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/CMakeLists.txt)
  include(${CMAKE_CURRENT_LIST_DIR}/tremolo/CMakeLists.txt)

  list(APPEND CC_EXTERNAL_LIBS 
    pvmp3dec
    ConvertUTF
    vorbisidec
  )

  if(ANDROID_NDK)
    list(APPEND CC_EXTERNAL_SROUCES
      ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
    )
  else()
    list(APPEND CC_EXTERNAL_SROUCES
      ${CMAKE_ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c
    )
  endif()

elseif(WINDOWS)
  #use source instead of library
  list(APPEND CC_EXTERNAL_SROUCES 
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTF.c
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTF.h
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTFWrapper.cpp
  )

  set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTF.c PROPERTIES
    LANGUAGE C
  )

elseif(APPLE)
  list(APPEND CC_EXTERNAL_SROUCES 
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTF.c
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTF.h
    ${CMAKE_CURRENT_LIST_DIR}/ConvertUTF/ConvertUTFWrapper.cpp
  )
  if(USE_SOCKET)
      include(${CMAKE_CURRENT_LIST_DIR}/SocketRocket/CMakeLists.txt)
      #list(APPEND CC_EXTERNAL_SROUCES 
      #  ${SOCKET_ROCKET_SOURCES}
      #)
      #list(APPEND CC_EXTERNAL_INCLUDES
      #  ${CMAKE_CURRENT_LIST_DIR}/SocketRocket
      #  ${CMAKE_CURRENT_LIST_DIR}/SocketRocket/Internal
      #  ${CMAKE_CURRENT_LIST_DIR}/SocketRocket/Utilities
      #)
      list(APPEND CC_EXTERNAL_LIBS
          SocketRocket
      )
  endif()
endif()