
add_library(crypto STATIC IMPORTED GLOBAL)
set_target_properties(crypto PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libcrypto.a
)

add_library(freetype STATIC IMPORTED GLOBAL)
set_target_properties(freetype PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libfreetype.a
)

add_library(jpeg STATIC IMPORTED GLOBAL)
set_target_properties(jpeg PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libjpeg.a
)

add_library(png STATIC IMPORTED GLOBAL)
set_target_properties(png PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libpng.a
)

add_library(ssl STATIC IMPORTED GLOBAL)
set_target_properties(ssl PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libssl.a
)

add_library(tiff STATIC IMPORTED GLOBAL)
set_target_properties(tiff PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libtiff.a
)

add_library(uv STATIC IMPORTED GLOBAL)
set_target_properties(uv PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libuv.a
)

add_library(webp STATIC IMPORTED GLOBAL)
set_target_properties(webp PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libwebp.a
)

add_library(websockets STATIC IMPORTED GLOBAL)
set_target_properties(websockets PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libwebsockets.a
)

add_library(z STATIC IMPORTED GLOBAL)
set_target_properties(z PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/libz.a
)

set(V8_LIBS
    v8_libbase 
    v8_libplatform 
    v8_libsampler 
    v8_snapshot 
    v8_compiler
   # v8_inspector 
    v8_base_without_compiler
)

add_library(v8_libbase STATIC IMPORTED GLOBAL)
set_target_properties(v8_libbase PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libbase.a
)


add_library(v8_libplatform STATIC IMPORTED GLOBAL)
set_target_properties(v8_libplatform PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libplatform.a
)

add_library(v8_libsampler STATIC IMPORTED GLOBAL)
set_target_properties(v8_libsampler PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libsampler.a
)

add_library(v8_snapshot STATIC IMPORTED GLOBAL)
set_target_properties(v8_snapshot PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_snapshot.a
)

add_library(v8_inspector STATIC IMPORTED GLOBAL)
set_target_properties(v8_inspector PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libinspector.a
)

add_library(v8_compiler STATIC IMPORTED GLOBAL)
set_target_properties(v8_compiler PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_compiler.a
)

add_library(v8_base_without_compiler STATIC IMPORTED GLOBAL)
set_target_properties(v8_base_without_compiler PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_base_without_compiler.a
)


# set(V8_ARCHIVE_DIR ${CMAKE_CURRENT_BINARY_DIR}/v8_tmp)

# #add_custom_target(v8_static_build
# add_custom_command(OUTPUT ${V8_ARCHIVE_DIR}/v8_static.a
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${V8_ARCHIVE_DIR}
#     COMMAND ar -x $<TARGET_FILE:v8_libbase>
#     COMMAND ar -x $<TARGET_FILE:v8_libplatform>
#     COMMAND ar -x $<TARGET_FILE:v8_libsampler>
#     COMMAND ar -x $<TARGET_FILE:v8_snapshot>
#     COMMAND ar -x $<TARGET_FILE:v8_compiler>
#     COMMAND ar -x $<TARGET_FILE:v8_base_without_compiler>
#     COMMAND ar -qcs ${V8_ARCHIVE_DIR}/v8_static.a *.o
#     WORKING_DIRECTORY ${V8_ARCHIVE_DIR}
#     DEPENDS ${V8_LIBS}
# )


add_library(v8_static STATIC IMPORTED GLOBAL)
#add_dependencies(v8_static v8_static_build)
set_target_properties(v8_static PROPERTIES
  #IMPORTED_LOCATION ${V8_ARCHIVE_DIR}/v8_static.a
  IMPORTED_LOCATION ${platform_spec_path}/v8/v8_static.a
)

list(APPEND CC_EXTERNAL_LIBS
    websockets
    ssl
    freetype
    crypto
    #curl
    jpeg
    png
    tiff
    uv
    webp
    z
    OpenSLES
    v8_static
)

if(CC_V8_DEBUGGER)
    list(APPEND CC_EXTERNAL_LIBS
        v8_inspector
    )
endif()