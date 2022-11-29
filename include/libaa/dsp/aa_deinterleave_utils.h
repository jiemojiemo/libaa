//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_DEINTERLEAVE_UTILS_H
#define LIBAA_AA_DEINTERLEAVE_UTILS_H

#include <cstddef>
namespace libaa {
namespace DeinterleaveUtils {
template <typename T>
void planarToInterleave(const T *const *input_buffers, size_t num_channels, size_t num_frames,
                        T *dst) {
    for (auto c = 0u; c < num_channels; ++c) {
        for (auto i = 0u; i < num_frames; ++i) {
            dst[i * num_channels + c] = input_buffers[c][i];
        }
    }
}
} // namespace DeinterleaveUtils
} // namespace libaa

#endif // LIBAA_AA_DEINTERLEAVE_UTILS_H
