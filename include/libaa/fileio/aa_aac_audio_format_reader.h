
//
// Created by William.Hua on 2021/3/24.
//

#pragma once
#include "libaa/fileio/aa_audio_format_reader.h"
namespace libaa
{
class AACAudioFormatReader : public AudioFormatReader
{
public:
    explicit AACAudioFormatReader(std::unique_ptr<InputStream> in_stream);

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
