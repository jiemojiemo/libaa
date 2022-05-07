
//
// Created by William.Hua on 2021/8/6.
//

#ifndef LIBAA_INCLUDE_LIBAA_DSP_AA_EXPONENTIAL_SMOOTHER_H
#define LIBAA_INCLUDE_LIBAA_DSP_AA_EXPONENTIAL_SMOOTHER_H
#pragma once

namespace libaa {
class ExponentialSmoother {
public:
    explicit ExponentialSmoother(float alpha)
        : alpha_(alpha) {}

    float getAlpha() const {
        return alpha_;
    }

    void setAlpha(float new_alpha) {
        alpha_ = new_alpha;
    }

    float process(float input_val) {
        float result =
            (input_val - pre_val_) * alpha_ + pre_val_; // S = a*y + (1-a)*S'
        pre_val_ = result;
        return result;
    }

private:
    float alpha_{0.0f};
    float pre_val_{0.0f};
};

} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_DSP_AA_EXPONENTIAL_SMOOTHER_H
