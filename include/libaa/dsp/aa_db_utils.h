
//
// Created by William.Hua on 2021/7/21.
//

#ifndef LIBAA_SRC_DSP_AA_DB_UTILS_H
#define LIBAA_SRC_DSP_AA_DB_UTILS_H
#pragma once
#include <cmath>
#include <limits>
namespace libaa {
class DbUtils {
public:
    static inline float dbToScale(float db) {
        return powf(10.0f, db / 20.0f);
    }

    static inline float scaleToDb(float scale) {
        if (scale < std::numeric_limits<float>::min()) {
            return 0;
        }
        return 20 * log10(scale);
    }
};

} // namespace libaa

#endif // LIBAA_SRC_DSP_AA_DB_UTILS_H
