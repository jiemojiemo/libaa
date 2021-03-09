
add_executable(load_file load_file.cpp)
target_link_libraries(load_file libaa)

add_executable(stft stft.cpp)
target_link_libraries(stft libaa)

add_executable(features features.cpp)
target_link_libraries(features libaa)

add_executable(audio_effect audio_effect.cpp)
target_link_libraries(audio_effect libaa)

add_executable(signal_generator signal_generator.cpp)
target_link_libraries(signal_generator libaa)

add_executable(play_sound play_sound.cpp)
target_link_libraries(play_sound libaa portaudio)

add_executable(audio_writer audio_writer.cpp)
target_link_libraries(audio_writer libaa)


