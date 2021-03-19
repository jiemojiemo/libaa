
//
// Created by William.Hua on 2021/2/23.
//

#ifndef LIBAA_SRC_FILEIO_AA_MP3_AUDIO_FORMAT_READER_H
#define LIBAA_SRC_FILEIO_AA_MP3_AUDIO_FORMAT_READER_H
#include "libaa/fileio/aa_audio_format_reader.h"
#include "libaa/fileio/aa_input_stream.h"

namespace libaa
{
class Mp3AudioFormatReader : public AudioFormatReader
{
public:
    explicit Mp3AudioFormatReader(std::unique_ptr<InputStream> in);

    ~Mp3AudioFormatReader() = default;

    bool isOpenOk() override;

    bool readSamples(float **dest_channels,
                     int num_dest_channels,
                     int start_offset_of_dest,
                     int64_t start_offset_of_file,
                     int num_samples) override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}

#endif //LIBAA_SRC_FILEIO_AA_MP3_AUDIO_FORMAT_READER_H
