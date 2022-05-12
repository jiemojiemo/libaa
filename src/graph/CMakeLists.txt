set(GRAPH_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_processor_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_graph_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_base_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_change_port.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_parameter_change_connection.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_node_serialization_utilities.cpp
        )

set(GRAPH_SRC ${GRAPH_SRC} PARENT_SCOPE)