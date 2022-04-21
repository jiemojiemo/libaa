add_libaa_test(aa_dsp_unit_test
        test_delay_line.cpp
        test_delay_line_array.cpp
        test_dtw.cpp
        test_features.cpp
        test_fft.cpp
        #        test_IIR_filters_coeffs.cpp
        test_lfo.cpp
        test_lfo_generator.cpp
        test_window.cpp
        test_yin.cpp
        test_exponential_smoother.cpp
        test_level_detector.cpp
        test_simple_delay.cpp
        test_simple_LPF.cpp
        test_comb_filter.cpp
        test_delay_APF.cpp
        test_nested_delay_APF.cpp
        test_two_band_shelving_filter.cpp
        test_iir_filter.cpp
        test_biquad_impl.cpp
        )

target_include_directories(aa_dsp_unit_test PRIVATE "${LIBAA_3RD_DIR}/Eigen3.3.7")