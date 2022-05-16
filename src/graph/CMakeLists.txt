set(GRAPH_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_processor_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_graph_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_base_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_change_port.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_change_connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_node_serialization_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_node_utilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_graph_builder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_realtime_graph_render.cpp
        )

set(GRAPH_SRC ${GRAPH_SRC} PARENT_SCOPE)