//
// Created by user on 3/22/22.
//

#ifndef AUDIOEFFECT_AE_MATH_TOOLS_H
#define AUDIOEFFECT_AE_MATH_TOOLS_H

#pragma once
#include <algorithm>
namespace libaa {
template <typename T> T findNextPower2Number(T v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

bool approximatelyEqual(float a, float b, float epsilon);

} // namespace libaa

#endif // AUDIOEFFECT_AE_MATH_TOOLS_H
