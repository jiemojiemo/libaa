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
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_filter_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_level_detector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_simple_delay.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_resampler.cpp
        )

set(DSP_SRC ${DSP_SRC} PARENT_SCOPE)