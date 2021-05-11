
//
// Created by William.Hua on 2021/3/9.
//

#pragma once
#include "libaa/fileio/aa_output_stream.h"
#include <ostream>

namespace libaa
{
class AudioFormatWriter
{
public:
    AudioFormatWriter(std::unique_ptr<OutputStream> out_stream,
                      int sample_rate,
                      int num_channels,
                      int num_bits):
        out_stream_(std::move(out_stream))
    {
        (void)sample_rate;
        (void)num_channels;
        (void)num_bits;
    }

    ~AudioFormatWriter() = default;

    virtual bool writePlanar(const float** samples, int num_samples) = 0;
    virtual bool writeInterleave(const float* samples, int num_samples) = 0;
    virtual bool isOpen() const = 0;

    virtual void flush() = 0;

    virtual void close() = 0;

public:
    std::unique_ptr<OutputStream> out_stream_;

    int sample_rate{-1};
    int num_channels{-1};
    int num_bits{-1};
};

}
