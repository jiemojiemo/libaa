//
// Created by user on 4/18/22.
//

#ifndef LIBAA_AA_NESTED_DELAY_APF_H
#define LIBAA_AA_NESTED_DELAY_APF_H

#pragma once
#include "libaa/dsp/aa_delay_APF.h"
#include "libaa/dsp/aa_simple_delay.h"
namespace libaa {
class NestedDelayAPF {
public:
    void prepare(float sample_rate, size_t max_delay_line_size) {
        inner_apf_.prepare(sample_rate, max_delay_line_size);

        delay_.prepare(sample_rate);
        delay_.resize(max_delay_line_size);
        lfo_.prepare(sample_rate);
    }

    struct NestedDelayAPFParameters : DelayAPF::DelayAPFParameters {
        float inner_apf_delay_ms{0.0f};
        float inner_apf_g{0.0f};
    };
    void updateParameters(NestedDelayAPFParameters params) {
        delay_.updateParameters(params);
        lpf_.updateParameters(params);
        lfo_.updateFrequencyHz(params.lfo_rate_hz);

        DelayAPF::DelayAPFParameters inner_apf_params =
            inner_apf_.getParameters();
        inner_apf_params.apf_g = params.inner_apf_g;
        inner_apf_params.delay_ms = params.inner_apf_delay_ms;
        inner_apf_.updateParameters(inner_apf_params);

        params_ = params;
    }

    float processSample(float in) {
        float wnD = 0.0f;
        if (params_.enable_LFO) {
            auto lfo_output = lfo_.renderAudioOutput();
            float max_delay = params_.delay_ms;
            float min_delay = max_delay - params_.lfo_max_modulation_ms;
            min_delay = std::fmax(0.0f, min_delay);

            float mod_delay_ms = unipolarMapTo(
                bipolarToUnipolar(lfo_output.normal_output * params_.lfo_depth),
                min_delay, max_delay);

            wnD = delay_.readDelayAt(mod_delay_ms);
        } else {
            wnD = delay_.readDelay();
        }

        if (params_.enable_LPF) {
            wnD = lpf_.processSample(wnD);
        }

        const float &apf_g = params_.apf_g;

        float wn = in + apf_g * wnD;
        float inner_y = inner_apf_.processSample(wn);
        float y = -apf_g * wn + wnD;

        delay_.writeSample(inner_y);
        return y;
    }

    const NestedDelayAPFParameters &getParameters() {
        return params_;
    }

    const SimpleDelay &getOuterSimpleDelay() const {
        return delay_;
    }

    const SimpleLPF &getOuterLPF() const {
        return lpf_;
    }

    const DelayAPF &getInnerAPF() const {
        return inner_apf_;
    }

private:
    NestedDelayAPFParameters params_;
    DelayAPF inner_apf_;
    SimpleDelay delay_;
    SimpleLPF lpf_;
    LFOGenerator lfo_;
};
} // namespace libaa

#endif // LIBAA_AA_NESTED_DELAY_APF_H
