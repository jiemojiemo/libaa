set(DSP_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_fft.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_stft.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_mfcc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_yin.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_distance.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_dtw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_lfo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_dsp_utilities.cpp

        ${LIBAA_3RD_DIR}/kissfft/kiss_fft.c
        ${LIBAA_3RD_DIR}/kissfft/kiss_fftr.c
        )

set(DSP_SRC ${DSP_SRC} PARENT_SCOPE)