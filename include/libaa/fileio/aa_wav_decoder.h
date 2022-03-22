//
// Created by william on 2019/11/27.
//
#pragma once

#include "aa_audio_decoder.h"
#include <memory>
namespace libaa {
class WavDecoder : public AudioDecoder {
public:
    WavDecoder();
    virtual ~WavDecoder() = default;
    int open(const std::string &filename) override;
    int read(float *buffer, size_t size) override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa
