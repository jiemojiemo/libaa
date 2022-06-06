set(CORE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_processor_parameter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_processor_parameters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_block.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_change.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_changes.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_math_tools.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_resource_path_finder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_clip.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_clip_factory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_json_utilities.cpp
        )

set(CORE_SRC ${CORE_SRC} PARENT_SCOPE)