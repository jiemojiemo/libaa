cmake_minimum_required(VERSION 3.10)

project(libaa LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")


# must return type
add_compile_options(-Werror=return-type)

add_subdirectory(third_party)

include_directories(include)
include_directories(third_party/dr_libs)
include_directories(third_party/kissfft)
include_directories(third_party/Eigen3.3.7)

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
        )

set(AUDIO_EFFECT_DIR
        src/audio_effect/aa_delay_effect.cpp)

add_library(libaa ${FILEIO_SRC} ${DSP_SRC} ${AUDIO_EFFECT_DIR})

add_subdirectory(example)

enable_testing()
add_subdirectory(test)