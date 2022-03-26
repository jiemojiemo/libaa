
//
// Created by William.Hua on 2021/7/25.
//

#include "aa_testing_helper.h"

namespace libaa {
bool operator==(const AudioBufferX<float> &lhs,
                const AudioBufferX<float> &rhs) {
    if (lhs.getNumberFrames() != rhs.getNumberFrames() ||
        lhs.getNumberChannels() != rhs.getNumberChannels()) {
        return false;
    }

    for (auto c = 0u; c < lhs.getNumberChannels(); ++c) {
        for (auto i = 0u; i < lhs.getNumberFrames(); ++i) {
            if (lhs.getReadPointer(c)[i] != rhs.getReadPointer(c)[i])
                return false;
        }
    }

    return true;
}

bool operator==(const ParameterChangePoint &lhs,
                const ParameterChangePoint &rhs) {
    return fabs((lhs.time - rhs.time)) < 1e-5 &&
           fabs((lhs.normalized_value - rhs.normalized_value)) < 1e-5;
}

} // namespace libaa
