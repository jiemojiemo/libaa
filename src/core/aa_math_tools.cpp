//
// Created by user on 3/24/22.
//
#include "libaa/core/aa_math_tools.h"

namespace libaa {
bool approximatelyEqual(float a, float b, float epsilon) {
    using namespace std;

    return abs(a - b) <= (max(abs(a), abs(b)) * epsilon);
}
} // namespace libaa