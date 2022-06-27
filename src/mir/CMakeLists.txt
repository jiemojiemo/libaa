set(MIR_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_fft.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_magnitude.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_max_filter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_mir_algorithm_base.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_mir_algorithm_factory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_moving_average.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_spectrum.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_superflux_novelty.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_superflux_peaks.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_triangular_bands.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_windowing.cpp
        )
set(MIR_SRC ${MIR_SRC} PARENT_SCOPE)