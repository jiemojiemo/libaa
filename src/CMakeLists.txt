add_subdirectory(audio_effect)
add_subdirectory(dsp)
add_subdirectory(fileio)

add_library(libaa ${FILEIO_SRC} ${DSP_SRC} ${AUDIO_EFFECT_DIR})
target_include_directories(libaa
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${LIBAA_INC_DRI}>
        ${LIBAA_3RD_DIR}/Eigen3.3.7
        PRIVATE
        ${LIBAA_3RD_DIR}/dr_libs
        ${LIBAA_3RD_DIR}/kissfft
        )