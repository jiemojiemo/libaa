cmake_minimum_required(VERSION 3.10)

project(libaa LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")


# must return value if you define non-void function
add_compile_options(-Werror=return-type)

# warning as error
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-function -Wno-unused-parameter -Wno-missing-braces )

add_subdirectory(third_party)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

option(COVERAGE "Use coverage flag" OFF)
if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

set(FILEIO_SRC
        src/fileio/aa_wav_decoder.cpp
        src/fileio/aa_audio_file.cpp)

set(DSP_SRC
        src/dsp/aa_fft.cpp
        third_party/kissfft/kiss_fft.c
        third_party/kissfft/kiss_fftr.c
        src/dsp/aa_stft.cpp
        src/dsp/aa_window.cpp
        src/dsp/aa_mfcc.cpp
        src/dsp/aa_yin.cpp
        src/dsp/aa_distance.cpp
        src/dsp/aa_dtw.cpp
        src/dsp/aa_lfo.cpp
        )

set(AUDIO_EFFECT_DIR
        src/audio_effect/aa_audio_effect_processor.cpp
        src/audio_effect/aa_delay_effect.cpp
        src/audio_effect/aa_vibrato_effect.cpp
        src/audio_effect/aa_flanger.cpp
        src/audio_effect/aa_chorus.cpp
        src/audio_effect/aa_tremolo.cpp
        src/audio_effect/aa_compressor.cpp
        src/audio_effect/aa_distortion.cpp
        src/audio_effect/aa_robot.cpp
        src/audio_effect/aa_biquad_filter.cpp
        )

add_library(libaa ${FILEIO_SRC} ${DSP_SRC} ${AUDIO_EFFECT_DIR})
target_include_directories(libaa
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Eigen3.3.7
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/dr_libs
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/kissfft
        )

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)