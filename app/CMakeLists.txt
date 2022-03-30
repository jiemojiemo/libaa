add_executable(regression_demo main.cpp)
target_link_libraries(regression_demo
        PRIVATE
        libaa
        yaml-cpp
        )
target_include_directories(regression_demo PRIVATE
        "${AUDIO_EFFECT_ROOT_DIR}/3rdparty/filesystem/include"
        )