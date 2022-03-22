//
// Created by william on 2019/12/16.
//

#include "libaa/dsp/aa_yin.h"

namespace libaa {
Yin::Yin(size_t block_size, size_t sample_rate, float threshold)
    : block_size_(block_size), half_size_(block_size / 2),
      sample_rate_(sample_rate), threshold_(threshold) {
    yin_buffer_.resize(half_size_);
}

float Yin::getPitch(const float *data) {
    // diffrence function

    difference(data);
    cumulativeMeanNormaliztion();
    int tau = absoluteThreshold();
    float better_tau = parabolicInterpolation(tau);

    return float(sample_rate_) / float(better_tau);
}

void Yin::difference(const float *data) {
    Map<const ArrayXf> x(data, block_size_);
    ArrayXf reversed_seq = x.head(half_size_).reverse();

    ArrayXf power_sum = ArrayXf::Zero(half_size_);
    power_sum(0) = reversed_seq.square().sum();

    ArrayXf x_square = x.square();
    for (size_t i = 1; i < half_size_; ++i) {
        power_sum(i) =
            power_sum(i - 1) - x_square(i - 1) + x_square(i + half_size_ - 1);
    }

    // autocorelation
    auto conv_result = FFT::fftconvolve(x.data(), x.size(), reversed_seq.data(),
                                        reversed_seq.size());
    Map<ArrayXf> auto_cor(conv_result.data() + half_size_ - 1,
                          half_size_); // only the interesting part

    // calucate d(tau)
    yin_buffer_ = power_sum(0) + power_sum - 2 * auto_cor;
}

void Yin::cumulativeMeanNormaliztion() {
    yin_buffer_(0) = 1;

    float cumsum = 0.0;
    for (int tau = 1; tau < yin_buffer_.size(); ++tau) {
        cumsum += yin_buffer_(tau);

        yin_buffer_(tau) = (yin_buffer_(tau) * float(tau)) / cumsum;
    }
}

int Yin::absoluteThreshold() {
    int tau = 0;
    for (tau = 1; tau < yin_buffer_.size(); ++tau) {
        if (yin_buffer_(tau) < threshold_) {
            // find the local minimum
            for (; tau + 1 < yin_buffer_.size() &&
                   yin_buffer_(tau + 1) < yin_buffer_(tau);
                 ++tau)
                ;

            return tau;
        }
    }

    if (tau == yin_buffer_.size()) {
        // find the global min if cannot find the local min
        int min_idx = -1;
        yin_buffer_.minCoeff(&min_idx);
        return min_idx;
    }

    return tau;
}

float Yin::parabolicInterpolation(int tau) {
    if (tau < kMinTau)
        return float(sample_rate_);

    if (tau == yin_buffer_.size() - 1)
        return float(tau);

    float s0 = yin_buffer_(tau - 1);
    float s1 = yin_buffer_(tau);
    float s2 = yin_buffer_(tau + 1);

    float better_tau = float(tau) - ((s2 - s0) / (2 * (s2 - s0 - 2 * s1)));

    return better_tau;
}
} // namespace libaa