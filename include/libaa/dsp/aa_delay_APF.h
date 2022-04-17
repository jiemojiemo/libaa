//
// Created by user on 4/17/22.
//

#ifndef LIBAA_AA_DELAY_APF_H
#define LIBAA_AA_DELAY_APF_H

#pragma once
#include "libaa/audio_generator/aa_lfo_generator.h"
#include "libaa/core/aa_math_tools.h"
#include "libaa/dsp/aa_simple_LPF.h"
#include "libaa/dsp/aa_simple_delay.h"
namespace libaa {
class DelayAPF {
public:
    void prepare(float sample_rate, size_t max_delay_line_size) {
        lfo_.prepare(sample_rate);

        delay_.prepare(sample_rate);
        delay_.resize(max_delay_line_size);
    }

    float processSample(float in) {
        float wnD = 0.0f;

        if (params_.enable_LFO) {
            auto lfo_output = lfo_.renderAudioOutput();
            float max_delay = params_.delay_params.delay_ms;
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

        float wn = in + params_.apf_g * wnD;
        float y = -params_.apf_g * wn + wnD;

        delay_.writeSample(wn);

        return y;
    }

    struct DelayAPFParameters {
        SimpleDelay::SimpleDelayParameters delay_params;
        SimpleLPF::SimpleLPFParameters lpf_params;
        float apf_g{0.0f};
        bool enable_LPF = {false};
        bool enable_LFO = {false};
        float lfo_rate_hz{0.0f};
        float lfo_depth{1.0f};
        float lfo_max_modulation_ms{0.0f};
    };
    void updateParameters(DelayAPFParameters params) {
        delay_.updateParameters(params.delay_params);
        lpf_.updateParameters(params.lpf_params);
        params_ = params;
    }

    const DelayAPFParameters &getParameters() const {
        return params_;
    }

    const SimpleDelay &getSimpleDelay() const {
        return delay_;
    }

    const SimpleLPF &getLPF() const {
        return lpf_;
    }

private:
    DelayAPFParameters params_;
    SimpleDelay delay_;
    SimpleLPF lpf_;
    LFOGenerator lfo_;
};
} // namespace libaa

#endif // LIBAA_AA_DELAY_APF_H
