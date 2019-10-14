
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

#  v8_libbase v8_libplatform v8_libsampler v8_snapshot v8_compiler
add_library(v8_libplatform STATIC IMPORTED GLOBAL)
set_target_properties(v8_libplatform PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libplatform.a
)

add_library(v8_libbase STATIC IMPORTED GLOBAL)
set_target_properties(v8_libbase PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libbase.a
)

add_library(v8_libsampler STATIC IMPORTED GLOBAL)
set_target_properties(v8_libsampler PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_libsampler.a
)

add_library(v8_snapshot STATIC IMPORTED GLOBAL)
set_target_properties(v8_snapshot PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_snapshot.a
)

add_library(v8_compiler STATIC IMPORTED GLOBAL)
set_target_properties(v8_compiler PROPERTIES
  IMPORTED_LOCATION ${platform_spec_path}/v8/libv8_compiler.a
)


set(CC_EXTERNAL_LIBS
    freetype
    crypto
    curl
    jpeg
    png
    ssl
    tiff
    uv
    webp
    z
    v8_libbase v8_libplatform v8_libsampler v8_snapshot v8_compiler
    websockets
    cpu-features
    OpenSLES
    pvmp3dec
)