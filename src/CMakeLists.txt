add_subdirectory(audio_effect)
add_subdirectory(dsp)
add_subdirectory(core)
add_subdirectory(fileio)
add_subdirectory(processor)
set(GENERATOR_SRC audio_generator/aa_lfo_generator.cpp)

message(STATUS "DSP_SRC:${DSP_SRC}")

add_library(libaa
        ${FILEIO_SRC}
        ${DSP_SRC}
        ${CORE_SRC}
        ${GENERATOR_SRC}
        ${PROCESSOR_SRC}
        )
target_include_directories(libaa
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${LIBAA_INC_DRI}>
        ${LIBAA_3RD_DIR}/Eigen3.3.7
        )

target_link_libraries(libaa
        PRIVATE dr_libs kissfft farbot fdk-aac mp3lame Ring-Buffer)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(libaa PRIVATE
            -Wall -Wextra -pedantic -Werror)

    # ignore some error
    target_compile_options(libaa PUBLIC -Wno-missing-braces -Wno-deprecated-copy -Wno-unknown-warning-option)
endif ()

if (LIBAA_INSTALL)
    install(TARGETS libaa
            EXPORT libaa
            LIBRARY DESTINATION lib  # 动态库安装路径
            ARCHIVE DESTINATION lib  # 静态库安装路径
            )
    install(
            DIRECTORY ${LIBAA_INC_DRI}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
    )
endif ()