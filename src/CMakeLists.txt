add_subdirectory(audio_effect)
add_subdirectory(dsp)
add_subdirectory(fileio)
set(GENERATOR_SRC audio_generator/aa_lfo_generator.cpp)

message(STATUS "DSP_SRC:${DSP_SRC}")

add_library(libaa
        ${FILEIO_SRC}
        ${DSP_SRC}
        ${AUDIO_EFFECT_DIR}
        ${GENERATOR_SRC}
        )
target_include_directories(libaa
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${LIBAA_INC_DRI}>
        ${LIBAA_3RD_DIR}/Eigen3.3.7
        )

target_link_libraries(libaa PRIVATE dr_libs kissfft)