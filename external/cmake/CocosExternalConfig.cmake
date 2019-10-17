
# set friendly platform define
 if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
     set(WINDOWS TRUE)
     set(SYSTEM_STRING "Windows Desktop")
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
     set(ANDROID TRUE)
     set(SYSTEM_STRING "Android")
 elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
     if(IOS)
         set(APPLE TRUE)
         set(SYSTEM_STRING "IOS")
     else()
         set(APPLE TRUE)
         set(MACOSX TRUE)
         set(SYSTEM_STRING "Mac OSX")
     endif()
 endif()

if(IOS)
    set(platform_name ios)
    set(platform_spec_path ios)
elseif(ANDROID)
    set(platform_name android)
    set(platform_spec_path android/${ANDROID_ABI})
elseif(WINDOWS)
    set(platform_name win32)
    set(platform_spec_path win32)
elseif(MACOSX)
    set(platform_name mac)
    set(platform_spec_path mac)
endif()

set(platform_spec_path "${CMAKE_CURRENT_LIST_DIR}/../${platform_spec_path}")
