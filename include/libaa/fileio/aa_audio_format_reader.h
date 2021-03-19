
//
// Created by William.Hua on 2021/2/10.
//

#pragma once
#include "libaa/fileio/aa_input_stream.h"
#include <istream>

namespace libaa
{
class AudioFormatReader
{
public:

    explicit AudioFormatReader(std::unique_ptr<InputStream> in_stream):
        in_stream_(std::move(in_stream))
    {

    }

    ~AudioFormatReader() = default;

    virtual bool isOpenOk();

    virtual bool readSamples (float** dest_channels,
                              int num_dest_channels,
                              int start_offset_of_dest,
                              int64_t start_offset_of_file,
                              int num_samples) = 0;

    virtual int64_t getPosition() const;

public:
    std::unique_ptr<InputStream> in_stream_;

    double  sample_rate{-1.0};
    int64_t length_in_samples{0};
    int32_t num_channels{0};
    int32_t num_bits{0};
    int64_t pos_{0};
};
}
