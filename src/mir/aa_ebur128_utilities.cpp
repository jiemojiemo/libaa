//
// Created by user on 11/29/22.
//

#include "libaa/mir/aa_ebur128_utilities.h"
#include "ebur128.h"

namespace libaa::MIR {
Ebur128Utilities::~Ebur128Utilities() {
    cleanup();
}
void Ebur128Utilities::prepare(int num_channels, int sample_rate, int max_block_size) {
    cleanup();
    st = ebur128_init(num_channels, sample_rate, EBUR128_MODE_S | EBUR128_MODE_I | EBUR128_MODE_LRA | EBUR128_MODE_SAMPLE_PEAK);

    resizeBuffer(num_channels, max_block_size);
    peaks.resize(num_channels, 0.0f);
}

void Ebur128Utilities::cleanup() {
    if (st) {
        ebur128_destroy(&st);
        st = nullptr;
    }
}

bool Ebur128Utilities::process(const float *const *input_buffers, size_t num_channels, size_t num_frames) {
    // planar to interleave
    DeinterleaveUtils::planarToInterleave(input_buffers, num_channels, num_frames, interleave_buf.data());

    // ebur128 processing
    ebur128_add_frames_float(st, interleave_buf.data(), num_frames);

    return getLoudnessFeatures();
}

void Ebur128Utilities::resizeBuffer(int num_channels, int max_block_size) {
    interleave_buf.resize(num_channels * max_block_size, 0.0f);
}

bool Ebur128Utilities::getLoudnessFeatures() {
    auto failed = false;
    if (EBUR128_SUCCESS != ebur128_loudness_momentary(st, &momentary)) {
        failed = true;
    }

    if (EBUR128_SUCCESS != ebur128_loudness_shortterm(st, &shorterm_loudness)) {
        failed = true;
    }

    if (EBUR128_SUCCESS != ebur128_loudness_global(st, &global_integrated_loudness)) {
        failed = true;
    }

    if (EBUR128_SUCCESS != ebur128_relative_threshold(st, &relative_threshold)) {
        failed = true;
    }

    if (EBUR128_SUCCESS != ebur128_loudness_range(st, &relative_threshold)) {
        failed = true;
    }

    if (momentary > kSilenceThreshold && !failed) {
        for (auto c = 0u; c < st->channels; ++c) {
            if (EBUR128_SUCCESS != ebur128_prev_sample_peak(st, c, &peaks[c])) {
                failed = true;
                break;
            }
        }
    }

    return !failed;
}
} // namespace libaa::MIR
