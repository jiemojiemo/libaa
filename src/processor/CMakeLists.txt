set(PROCESSOR_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_delay_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_gain_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_delay_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_vibrator_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_vibrator_processor_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_reverb_tank_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_pitch_shifter_processor.cpp
        )

set(PROCESSOR_SRC ${PROCESSOR_SRC} PARENT_SCOPE)