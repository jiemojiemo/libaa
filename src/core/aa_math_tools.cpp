//
// Created by user on 3/24/22.
//
#include "libaa/core/aa_math_tools.h"

namespace libaa {
bool approximatelyEqual(float a, float b, float epsilon) {
    using namespace std;

    return abs(a - b) <= (max(abs(a), abs(b)) * epsilon);
}

float bipolarToUnipolar(float v) {
    return 0.5f * v + 0.5f;
}

float unipolarMapTo(float unipolar_v, float min_v, float max_v) {
    return min_v + (max_v - min_v) * unipolar_v;
}

} // namespace libaa