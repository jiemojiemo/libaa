add_libaa_test(
        aa_offline_unit_tests
        test_time_stretching.cpp
)

target_include_directories(aa_offline_unit_tests PRIVATE
        "${LIBAA_DIR}/third_party/rubberband-2.0.2/rubberband"
        "${LIBAA_DIR}/third_party/rubberband-2.0.2/src/"
        )