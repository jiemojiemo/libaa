add_libaa_test(aa_processor_unit_test
        test_gain_processor.cpp
        test_delay_processor_utilities.cpp
        test_delay_processor.cpp
        test_vibrator_processor.cpp
        test_reverb_tank_processor.cpp
        test_reverb_tank_processor_utilities.cpp
        test_pitch_shift_utilities.cpp
        test_pitch_shifter_processor.cpp
        #        test_audio_filter_utilities.cpp
        #        test_audio_filter_processor.cpp
        )

target_include_directories(aa_processor_unit_test PRIVATE
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/rubberband>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/src>
        )