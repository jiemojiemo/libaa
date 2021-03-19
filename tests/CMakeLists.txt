# test dsp
add_executable(unit_test
        aa_scope_wave_file.cpp

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
        test_wav_audio_format_reader.cpp
        test_mp3_audio_format_reader.cpp
        test_wav_audio_format_writer.cpp

        # audio effect
        test_audio_effect.cpp
        test_IIR_filters_coeffs.cpp

        # signal generator
        test_lfo_generator.cpp

        test_file_stream.cpp

        )
target_link_libraries(unit_test
        gmock_main
        libaa
        dr_libs
        )

add_test(
        NAME unit_test
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_test
)