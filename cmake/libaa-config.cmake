get_filename_component(libaa_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH ${libaa_CMAKE_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/fdk-aac/fdk-aac-targets.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/kissfft.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/mp3lame.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/libaa-targets.cmake")
