add_libaa_test(
        aa_app_unit_test
        test_demo_context.cpp
        test_demo_pipeline.cpp
        test_read_wav_value.cpp
        test_write_wav_value.cpp
        test_loop_pipeline.cpp
)
target_link_libraries(aa_app_unit_test PRIVATE yaml-cpp)
target_include_directories(aa_app_unit_test PRIVATE
        "${LIBAA_DIR}/app"
        )