
add_executable(load_file load_file.cpp)
target_link_libraries(load_file libaa)

#add_executable(stft stft.cpp)
#target_link_libraries(stft libaa)

#add_executable(features features.cpp)
#target_link_libraries(features libaa)

add_executable(audio_effect audio_effect.cpp)
target_link_libraries(audio_effect libaa)

add_executable(signal_generator signal_generator.cpp)
target_link_libraries(signal_generator libaa)

#add_executable(play_sound play_sound.cpp)
#target_link_libraries(play_sound PRIVATE libaa PortAudio)

add_executable(audio_writer audio_writer.cpp)
target_link_libraries(audio_writer libaa)

add_executable(wav_diff wav_diff.cpp)
target_link_libraries(wav_diff libaa)

add_executable(audio_processor audio_processor.cpp)
target_link_libraries(audio_processor libaa)

add_executable(graph_test graph_test.cpp)
target_link_libraries(graph_test libaa)



