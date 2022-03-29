add_library(dr_libs STATIC dr_libs.cpp)
target_include_directories(dr_libs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

if (BUILD_SHARED_LIBS)
    set_property(TARGET dr_libs PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ()
