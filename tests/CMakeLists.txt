# test dsp
add_executable(unit_test
        # dsp
        test_fft.cpp
        test_window.cpp
        test_yin.cpp
        test_dtw.cpp
        test_features.cpp
        test_lfo.cpp

        # audio basics
        test_audio_buffer.cpp
        test_delay_line.cpp
        test_delay_line_array.cpp

        # fileio
        test_audio_file.cpp

        # audio effect
        test_audio_effect.cpp
        test_IIR_filters_coeffs.cpp
        )
target_link_libraries(unit_test
        gmock_main
        libaa
        )

add_test(
        NAME unit_test
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_test
)