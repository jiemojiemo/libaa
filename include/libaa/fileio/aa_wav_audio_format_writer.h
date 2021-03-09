
//
// Created by William.Hua on 2021/3/9.
//

#pragma once
#include "libaa/fileio/aa_audio_format_writer.h"
#include <vector>
namespace libaa
{
class WavFormatWriter : public AudioFormatWriter
{
public:
    WavFormatWriter(std::ostream& out_stream,
                    int rate,
                    int channels,
                    int bits);

    bool writePlanar(const float **samples, int num_samples) override;

    bool writeInterleave(const float* samples, int num_samples) override;

    bool isOpen() const override;

    void flush() override;

    void close() override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

}
