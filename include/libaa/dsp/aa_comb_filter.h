//
// Created by user on 4/16/22.
//

#ifndef LIBAA_AA_COMB_FILTER_H
#define LIBAA_AA_COMB_FILTER_H

#pragma once
#include "libaa/core/aa_math_tools.h"
#include "libaa/dsp/aa_simple_delay.h"
namespace libaa {
class CombFilter {
public:
    struct CombFilterParameters {
        SimpleDelay::SimpleDelayParameters simple_delay_params{};
        float lpf_g{0.0f};
        bool enable_LPF{false};
        float rt_60_ms{0.0f};
    };

    void prepare(float sample_rate) {
        delay_.prepare(sample_rate);
        const int kMaxDelaySize = 2.0f * sample_rate; // 2s
        delay_.resize(kMaxDelaySize);
    }

    void updateParameters(CombFilterParameters params) {
        if (params_.rt_60_ms != params.rt_60_ms ||
            params_.simple_delay_params.delay_ms !=
                params.simple_delay_params.delay_ms) {
            float exponent = -3.0f * params.simple_delay_params.delay_ms;
            comb_g_ = std::powf(10.0f, exponent / params.rt_60_ms);
        }
        g2 = params.lpf_g * (1.0f - comb_g_);
        delay_.updateParameters(params.simple_delay_params);

        params_ = params;
    }

    const CombFilterParameters &getParameters() const {
        return params_;
    }

    float processSample(float in) {
        float y = delay_.readDelay();
        float x;

        if (params_.enable_LPF) {
            float filtered = y + g2 * state_;
            state_ = filtered;
            x = in + comb_g_ * filtered;
        } else {
            x = in + comb_g_ * y;
        }

        delay_.writeSample(x);

        return y;
    }

    float getCombFilterFeedback() const {
        return comb_g_;
    }

private:
    SimpleDelay delay_;
    CombFilterParameters params_;
    float comb_g_{0.0f};
    float g2{0.0f};
    float state_{0.0f};
};
} // namespace libaa

#endif // LIBAA_AA_COMB_FILTER_H
