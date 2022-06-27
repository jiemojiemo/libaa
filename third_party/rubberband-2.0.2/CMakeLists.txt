cmake_minimum_required(VERSION 3.13)

add_library(rubberband ${CMAKE_CURRENT_SOURCE_DIR}/single/RubberBandSingle.cpp)
target_link_libraries(rubberband PRIVATE SampleRate::samplerate)

if (APPLE OR IOS)
    target_link_libraries(rubberband PRIVATE "-framework Accelerate")
endif ()

target_compile_definitions(rubberband PRIVATE HAVE_LIBSAMPLERATE)
target_link_libraries(rubberband PRIVATE kissfft)

target_include_directories(rubberband
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rubberband>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/kissfft>
        )