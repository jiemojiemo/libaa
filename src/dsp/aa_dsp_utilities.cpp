
//
// Created by William.Hua on 2021/1/15.
//
#include "dsp/aa_dsp_utilities.h"
#include <cmath>
namespace libaa::DSPUtilities
{
bool isPowerOf2(size_t number)
{
    return (number & number - 1) == 0 && number > 0;
}


double parabolicSine(double angle){
    constexpr double B = 4.0 / M_PI;
    constexpr double C = -4.0 / (M_PI*M_PI);
    constexpr double P = 0.225;

    double y = B * angle + C * angle * fabs(angle);
    y = P * (y * fabs(y) - y) + y;
    return y;
}

}
