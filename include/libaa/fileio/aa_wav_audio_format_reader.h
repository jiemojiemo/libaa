
//
// Created by William.Hua on 2021/2/10.
//

#pragma once
#include "aa_audio_format_reader.h"
namespace libaa
{
class WaveAudioFormatReader : public AudioFormatReader
{
public:
    explicit WaveAudioFormatReader(std::istream& in_stream);
    explicit WaveAudioFormatReader(std::unique_ptr<InputStream> in_stream);

    ~WaveAudioFormatReader() = default;

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
