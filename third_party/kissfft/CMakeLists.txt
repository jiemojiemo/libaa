add_library(kissfft kiss_fft.c kiss_fftr.c)
target_include_directories(kissfft
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

if (BUILD_SHARED_LIBS)
    set_property(TARGET kissfft PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ()