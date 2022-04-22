
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/dsp/aa_db_utils.h"
#include <cmath>
#include <limits>
namespace libaa {
float dbToScale(float db) {
    return powf(10.0f, db / 20.0f);
}

float scaleToDb(float scale) {
    if (scale < std::numeric_limits<float>::min()) {
        return 0;
    }
    return 20 * log10(scale);
}

} // namespace libaa