//
// Created by william on 2019/12/16.
//

#pragma once
#include "aa_fft.h"
#include <Eigen/Core>

namespace libaa {
using namespace Eigen;

class Yin {
public:
    Yin(size_t block_size, size_t sample_rate, float threshold);

    float getPitch(const float *data);

private:
    void difference(const float *data);

    void cumulativeMeanNormaliztion();

    int absoluteThreshold();

    float parabolicInterpolation(int tau);

private:
    size_t block_size_;
    size_t half_size_;
    size_t sample_rate_;
    float threshold_;
    ArrayXf yin_buffer_;

    static constexpr int kMinTau = 2;
};

} // namespace libaa
