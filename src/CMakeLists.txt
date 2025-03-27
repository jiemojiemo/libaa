add_subdirectory(audio_effect)
add_subdirectory(dsp)
add_subdirectory(core)
add_subdirectory(fileio)
add_subdirectory(graph)
add_subdirectory(mir)
add_subdirectory(processor)
set(GENERATOR_SRC audio_generator/aa_lfo_generator.cpp)

# install version file
include(GitUtils)
include(git_revision)
get_git_revision_info(
        libaa_GIT_SHORT_TAG
        libaa_GIT_LONG_TAG
        libaa_GIT_COMMIT_HASH)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_version.h.in
        ${CMAKE_BINARY_DIR}/aa_version.h)
file(COPY ${CMAKE_BINARY_DIR}/aa_version.h DESTINATION ${LIBAA_INC_DIR}/libaa)

add_library(libaa
        ${FILEIO_SRC}
        ${DSP_SRC}
        ${CORE_SRC}
        ${GENERATOR_SRC}
        ${PROCESSOR_SRC}
        ${GRAPH_SRC}
        ${MIR_SRC}
        offline/aa_time_stretching.cpp
        )
target_include_directories(libaa
        PUBLIC
        $<INSTALL_INTERFACE:include/libaa>
        $<BUILD_INTERFACE:${LIBAA_INC_DIR}>
        PRIVATE
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/Eigen3.3.7>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/farbot/include>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/Ring-Buffer>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/dr_libs>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/rubberband>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/src>
        )

target_link_libraries(libaa
        PUBLIC kissfft fdk-aac mp3lame rubberband nlohmann_json
        ebur128
        )

target_compile_features(libaa PUBLIC cxx_std_14)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(libaa PRIVATE
            -Wall -Wextra -pedantic -Werror)

    # ignore some error
    target_compile_options(libaa PRIVATE -Wno-missing-braces -Wno-deprecated-copy -Wno-unknown-warning-option -Wno-deprecated-declarations)
endif ()

if (LIBAA_INSTALL)
    install(TARGETS ebur128
            EXPORT ebur128
            LIBRARY DESTINATION lib  # 动态库安装路径
            ARCHIVE DESTINATION lib  # 静态库安装路径
            )
    install(EXPORT ebur128 DESTINATION lib/cmake)

    install(TARGETS kissfft
            EXPORT kissfft
            LIBRARY DESTINATION lib  # 动态库安装路径
            ARCHIVE DESTINATION lib  # 静态库安装路径
            )
    install(EXPORT kissfft DESTINATION lib/cmake)

    install(TARGETS fdk-aac
            EXPORT fdk-aac
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )

    install(TARGETS mp3lame
            EXPORT mp3lame
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(EXPORT mp3lame DESTINATION lib/cmake)

    install(TARGETS samplerate
            EXPORT samplerate
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(EXPORT samplerate DESTINATION lib/cmake)

    install(TARGETS nlohmann_json
            EXPORT nlohmann_json
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(EXPORT nlohmann_json DESTINATION lib/cmake)

    install(TARGETS rubberband
            EXPORT rubberband
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(EXPORT rubberband DESTINATION lib/cmake)

    install(TARGETS libaa
            EXPORT libaa-targets
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
    install(
            DIRECTORY ${LIBAA_INC_DIR}/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
    )

    install(EXPORT libaa-targets
            FILE
            libaa-targets.cmake
            NAMESPACE
            libaa::
            DESTINATION
            "lib/cmake/"
            )

    install(
            FILES ${CMAKE_BINARY_DIR}/aa_version.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

endif ()