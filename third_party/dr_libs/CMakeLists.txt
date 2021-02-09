add_library(dr_libs dr_libs.cpp)
target_include_directories(dr_libs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )