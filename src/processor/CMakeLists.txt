set(PROCESSOR_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_delay_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_gain_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_delay_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_iir_filter_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_resampler_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_vibrato_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_vibrato_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_reverb_tank_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_pitch_shifter_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_source_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_source_callback_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_processor_factory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_compressor_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_limiter_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_channel_vocoder_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_channel_vocoder_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_time_stretching_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_loudness_normalization_processor.cpp
        )

set(PROCESSOR_SRC ${PROCESSOR_SRC} PARENT_SCOPE)