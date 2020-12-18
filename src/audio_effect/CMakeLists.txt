set(AUDIO_EFFECT_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_effect_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_delay_effect.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_vibrato_effect.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_flanger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_chorus.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_tremolo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_compressor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_distortion.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_robot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_biquad_filter.cpp
        )

set(AUDIO_EFFECT_DIR ${AUDIO_EFFECT_DIR} PARENT_SCOPE)