//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_EBUR128_UTILITIES_H
#define LIBAA_AA_EBUR128_UTILITIES_H

#include "ebur128.h"
#include "libaa/dsp/aa_deinterleave_utils.h"
#include <vector>
namespace libaa::MIR {
class Ebur128Utilities {
public:
    ~Ebur128Utilities();

    void prepare(int num_channels, int sample_rate, int max_block_size);

    void cleanup();

    bool process(const float *const *input_buffers, size_t num_channels, size_t num_frames);

    void resizeBuffer(int num_channels, int max_block_size);
    const std::vector<float> &getInternalInterleaveBuffer() const {
        return interleave_buf;
    }

    ebur128_state *st{nullptr};
    std::vector<float> interleave_buf;

    // features
    double momentary{0};
    double shorterm_loudness{0};
    double global_integrated_loudness{0};
    double relative_threshold{0};
    double range{0};
    std::vector<double> peaks;

private:
    bool getLoudnessFeatures();
    constexpr static float kSilenceThreshold = -70;
};
} // namespace libaa::MIR

#endif // LIBAA_AA_EBUR128_UTILITIES_H
