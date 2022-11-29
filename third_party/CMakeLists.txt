add_subdirectory(kissfft)
add_subdirectory(fdk-aac)
add_subdirectory(portaudio EXCLUDE_FROM_ALL)
add_subdirectory(libmp3lame)

if (NOT EXISTS samplerate)
    option(BUILD_TESTING OFF)
    add_subdirectory(libsamplerate-0.2.2)
endif ()
# build rubberband after samplerate since it depends on samplerate
if (NOT EXISTS rubberband)
    add_subdirectory(rubberband-2.0.2)
endif ()

if (LIBAA_BUILD_TESTING AND NOT EXISTS gmock)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif ()

if (NOT EXISTS yaml-cpp)
    set(YAML_CPP_BUILD_TESTS OFF)
    set(YAML_CPP_BUILD_TOOLS OFF)
    set(YAML_CPP_CLANG_FORMAT_EXE OFF)
    set(YAML_CPP_INSTALL OFF)
    add_subdirectory(yaml-cpp EXCLUDE_FROM_ALL)
endif ()

if (NOT EXISTS nlohmann_json)
    add_subdirectory(nlohmann/json EXCLUDE_FROM_ALL)
endif ()

add_library(Ring-Buffer INTERFACE)
target_include_directories(Ring-Buffer INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/Ring-Buffer/")

add_library(farbot INTERFACE)
target_include_directories(farbot INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/farbot/include")

if (NOT EXISTS eigen)
    add_library(eigen INTERFACE)
    target_include_directories(eigen INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Eigen3.3.7>
            )
endif ()
