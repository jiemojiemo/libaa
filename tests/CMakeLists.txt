add_library(aa_test_helper STATIC aa_scope_wave_file.cpp aa_testing_helper.cpp)
target_include_directories(aa_test_helper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(aa_test_helper PUBLIC libaa gmock)

function(add_libaa_test unit_test_name)
    add_executable(${unit_test_name} ${ARGN})
    target_link_libraries(${unit_test_name} PRIVATE
            gmock_main
            libaa
            aa_test_helper
            )
    target_include_directories(${unit_test_name} PRIVATE
            ${LIBAA_DIR}/tests/common/include)

    add_test(
            NAME ${unit_test_name}
            WORKING_DIRECTORY ./
            COMMAND ${unit_test_name}
    )
endfunction()

add_subdirectory(app)
add_subdirectory(fileio)
add_subdirectory(core)
add_subdirectory(dsp)
add_subdirectory(graph)
add_subdirectory(mir)
add_subdirectory(processor)

