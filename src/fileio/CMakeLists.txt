set(FILEIO_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_decoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_mp3_audio_format_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aa_wav_audio_format_writer.cpp
        )

set(FILEIO_SRC ${FILEIO_SRC} PARENT_SCOPE)