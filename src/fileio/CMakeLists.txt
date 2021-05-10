set(FILEIO_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_decoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_mp3_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_audio_format_writer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_aac_audio_format_reader.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/aa_file_stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_out_file_stream.cpp
        )

set(FILEIO_SRC ${FILEIO_SRC} PARENT_SCOPE)