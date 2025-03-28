//
// Created by william on 2019/12/9.
//

#pragma once
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <gmock/gmock.h>
#include <vector>
using namespace testing;

MATCHER_P(FloatNearPointwise, tol, "Out of range") {
    return (std::get<0>(arg) > std::get<1>(arg) - tol &&
            std::get<0>(arg) < std::get<1>(arg) + tol);
}

MATCHER_P(NearWithPrecision, ferr, "") {
    return abs(get<0>(arg) - get<1>(arg)) < ferr;
}

namespace libaa {

MATCHER_P2(ComplexNearEqual, ferr, rhs, "") {
    return abs(arg - rhs) < ferr;
}

bool operator==(const AudioBufferX<float> &lhs, const AudioBufferX<float> &rhs);

class ScopeFile {
public:
    explicit ScopeFile(std::string output_path);
    virtual ~ScopeFile();

protected:
    std::string output_path_;
};

class ScopeWaveFile : public ScopeFile {
public:
    ScopeWaveFile(std::string output_path, size_t sample_rate,
                  size_t num_channel, size_t num_frames, size_t num_bits,
                  float fill_val);
    ScopeWaveFile(std::string output_path, size_t sample_rate,
                  const std::vector<std::vector<float>> &samples,
                  size_t num_bits);
};

class ScopeTextFile : public ScopeFile {
public:
    ScopeTextFile(std::string output_path, const std::string &txt);
};

std::unique_ptr<ScopeWaveFile> makeScopeWavFile(const std::string &file_uri);

} // namespace libaa
