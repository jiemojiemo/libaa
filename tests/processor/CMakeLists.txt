add_libaa_test(aa_processor_unit_test
        test_gain_processor.cpp
        test_delay_processor_utilities.cpp
        test_delay_processor.cpp
        test_vibrator_processor.cpp
        test_processor_utilities.cpp
        test_reverb_tank_processor.cpp
        test_reverb_tank_processor_utilities.cpp
        test_pitch_shift_utilities.cpp
        test_pitch_shifter_processor.cpp
        test_schroeder_reverb_processor.cpp
        test_moorer_reverb_processor.cpp
        test_source_processor.cpp
        test_source_callback_processor.cpp
        test_processor_factory.cpp
        test_dynamic_processor_utilities.cpp
        test_compressor_processor.cpp
        test_limiter_processor.cpp
        #        test_audio_filter_utilities.cpp
        #        test_audio_filter_processor.cpp
        )

target_include_directories(aa_processor_unit_test PRIVATE
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/rubberband>
        $<BUILD_INTERFACE:${LIBAA_3RD_DIR}/rubberband-2.0.2/src>
        )
target_link_libraries(aa_processor_unit_test PRIVATE
        nlohmann_json
        )