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
        $<INSTALL_INTERFACE:include/libaa>
        $<BUILD_INTERFACE:${LIBAA_INC_DRI}>
        PRIVATE
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/Eigen3.3.7>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/farbot/include>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/Ring-Buffer>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/dr_libs>
        )

target_link_libraries(libaa
        PRIVATE kissfft fdk-aac mp3lame)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(libaa PRIVATE
            -Wall -Wextra -pedantic -Werror)

    # ignore some error
    target_compile_options(libaa PUBLIC -Wno-missing-braces -Wno-deprecated-copy -Wno-unknown-warning-option)
endif ()

if (LIBAA_INSTALL)
    install(TARGETS kissfft
            EXPORT kissfft
            LIBRARY DESTINATION lib  # 动态库安装路径
            ARCHIVE DESTINATION lib  # 静态库安装路径
            )
    install(EXPORT kissfft DESTINATION lib/cmake/kissfft)

    #    install(TARGETS dr_libs
    #            EXPORT dr_libs
    #            LIBRARY DESTINATION lib
    #            ARCHIVE DESTINATION lib
    #            )
    #    install(EXPORT dr_libs DESTINATION lib/cmake/dr_libs)

    #    install(TARGETS fdk-aac
    #            EXPORT fdk-aac
    #            LIBRARY DESTINATION lib
    #            ARCHIVE DESTINATION lib
    #            )
    #    install(EXPORT fdk-aac DESTINATION lib/cmake/fdk-aac)

    install(TARGETS mp3lame
            EXPORT mp3lame
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(EXPORT mp3lame DESTINATION lib/cmake/mp3lame)

    install(TARGETS libaa
            EXPORT libaa
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(
            DIRECTORY ${LIBAA_INC_DRI}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
    )


    install(EXPORT libaa DESTINATION lib/cmake/libaa)


endif ()