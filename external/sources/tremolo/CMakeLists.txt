cmake_minimum_required(VERSION 3.8)

project(vorbisidec C)

set(VORIBISIDEC_SOURCE 
    Tremolo/bitwise.c
    Tremolo/codebook.c
    Tremolo/dsp.c
    Tremolo/floor0.c
    Tremolo/floor1.c
    Tremolo/floor_lookup.c
    Tremolo/framing.c
    Tremolo/info.c
    Tremolo/mapping0.c
    Tremolo/mdct.c
    Tremolo/misc.c
    Tremolo/res012.c
    Tremolo/vorbisfile.c
)


add_library(vorbisidec ${VORIBISIDEC_SOURCE})

target_include_directories(vorbisidec PUBLIC 
    Tremolo
)

target_compile_definitions(vorbisidec PRIVATE
    ONLY_C
)
