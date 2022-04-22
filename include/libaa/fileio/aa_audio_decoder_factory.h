//
// Created by william on 2019/12/1.
//

#pragma once
#include "aa_wav_decoder.h"
#include <iostream>
#include <string>

namespace libaa {
class AudioDecoderFactory {
public:
    static std::unique_ptr<AudioDecoder>
    createDecoder(const std::string &filename) {
        std::string::size_type idx = filename.rfind('.');
        std::string extension{"NoExtension"};
        if (idx != std::string::npos) {
            extension = filename.substr(idx + 1);
        }

        if (extension == "wav") {
            return std::make_unique<WavDecoder>();
        }

        std::cerr << "Unsupport " << extension << " audio format\n";
        return nullptr;
    }

    static std::vector<std::string> getSupportFormat() {
        return {"wav"};
    }

    static std::unique_ptr<AudioDecoder> createWaveDecoder() {
        return std::make_unique<WavDecoder>();
    }
};
} // namespace libaa