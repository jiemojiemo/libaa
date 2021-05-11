
//
// Created by William.Hua on 2021/5/11.
//

#ifndef LIBAA_INCLUDE_LIBAA_FILEIO_AA_MP3_AUDIO_FORMAT_WRITER_H
#define LIBAA_INCLUDE_LIBAA_FILEIO_AA_MP3_AUDIO_FORMAT_WRITER_H
#pragma once
#include "libaa/fileio/aa_audio_format_writer.h"
namespace libaa
{
class MP3FormatWriter : public AudioFormatWriter
{
public:
    MP3FormatWriter(std::unique_ptr<OutputStream> out_stream,
                    int rate,
                    int channels,
                    int bits);

    virtual ~MP3FormatWriter() = default;

    bool writePlanar(const float **samples, int num_samples) override;
    bool writeInterleave(const float *samples, int num_samples) override;

    void flush() override;
    void close() override;

    bool isOpen() const override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

}

#endif //LIBAA_INCLUDE_LIBAA_FILEIO_AA_MP3_AUDIO_FORMAT_WRITER_H
