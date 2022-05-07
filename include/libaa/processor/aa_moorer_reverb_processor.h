//
// Created by user on 5/2/22.
//

#ifndef LIBAA_AA_MOORER_REVERB_PROCESSOR_H
#define LIBAA_AA_MOORER_REVERB_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/dsp/aa_comb_filter.h"
#include "libaa/dsp/aa_delay_APF.h"
#include "libaa/dsp/aa_simple_delay.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <array>

namespace libaa {
class MoorerReverbProcessor : public IAudioProcessor {
public:
    MoorerReverbProcessor() {
        params_.pushFloatParameter("Reverb Time(ms)", 50, 0, 2000);
        params_.pushFloatParameter("PreDelay(ms)", 25.0f, 0.0f, 100.0f);
        params_.pushFloatParameter("Wet", 1.0, 0, 1.0);
        params_.pushFloatParameter("Dry", 0.0, 0, 1.0);
    }
    ~MoorerReverbProcessor() override = default;
    void prepareToPlay(float sample_rate, int max_block_size) override {
        const int max_delay_size = 1.0 * sample_rate;

        pre_delay_.prepare(sample_rate, max_delay_size);

        for (int i = 0; i < comb_filters_.size(); ++i) {
            comb_filters_[i].prepare(sample_rate, max_delay_size);
        }

        for (int i = 0; i < allpass_filters_.size(); ++i) {
            allpass_filters_[i].prepare(sample_rate, max_delay_size);
        }

        updateSimpleDelayParameter();
        updateCombFilterParameter(params_.get(0).getPlainValue());
        updateAPFParameter();
    }
    std::string getName() const override {
        return {"MoorerReverb"};
    }
    void processBlock(AudioBlock *input, AudioBlock *output) override {
        output->buffer.copyFrom(&input->buffer);

        ProcessorUtilities::updateParameterFromParameterChanges(
            input->param_changes, params_);
        updateCombFilterParameter(params_.get(0).getPlainValue());
        updateSimpleDelayParameter();

        auto num_channels = output->buffer.getNumberChannels();
        float *left_channel = output->buffer.getWriterPointer(0);
        float *right_channel = num_channels > 1
                                   ? output->buffer.getWriterPointer(1)
                                   : left_channel;

        const float wet = params_.get(2).getPlainValue();
        const float dry = params_.get(3).getPlainValue();
        for (size_t i = 0; i < output->buffer.getNumberFrames(); ++i) {
            float xn_L = left_channel[i];
            float xn_R = right_channel[i];
            float mono_xn = (xn_L + xn_R) * 0.5f;

            const float w = 0.707;
            float early_reflection_output = pre_delay_.processSample(mono_xn);
            if (early_reflection_output != 0) {
                early_reflection_output = early_reflection_output;
            }

            early_reflection_output += w * pre_delay_.readDelayAtPercentage(23);
            early_reflection_output -= w * pre_delay_.readDelayAtPercentage(41);
            early_reflection_output += w * pre_delay_.readDelayAtPercentage(59);
            early_reflection_output -= w * pre_delay_.readDelayAtPercentage(73);

            early_reflection_output -= w * pre_delay_.readDelayAtPercentage(29);
            early_reflection_output += w * pre_delay_.readDelayAtPercentage(43);
            early_reflection_output -= w * pre_delay_.readDelayAtPercentage(61);
            early_reflection_output += w * pre_delay_.readDelayAtPercentage(79);

            float comb_output = 0.0f;
            comb_output +=
                comb_filters_[0].processSample(early_reflection_output);
            comb_output -=
                comb_filters_[1].processSample(early_reflection_output);
            comb_output +=
                comb_filters_[2].processSample(early_reflection_output);
            comb_output -=
                comb_filters_[3].processSample(early_reflection_output);
            comb_output +=
                comb_filters_[4].processSample(early_reflection_output);
            comb_output -=
                comb_filters_[5].processSample(early_reflection_output);

            float apf_output = comb_output;
            for (auto &apf : allpass_filters_) {
                apf_output = apf.processSample(apf_output);
            }

            float wet_output = early_reflection_output + apf_output;
            float output = mono_xn * dry + wet * wet_output;
            if (num_channels == 1) {
                left_channel[i] = output;
            } else {
                left_channel[i] = output;
                right_channel[i] = output;
            }
        }
    }
    const AudioProcessorParameters *getParameters() const override {
        return &params_;
    }

private:
    void updateCombFilterParameter(float new_rt_60) {
        for (int i = 0; i < comb_filters_.size(); ++i) {
            auto param = comb_filters_[0].getParameters();
            param.enable_LPF = true;
            param.rt_60_ms = new_rt_60;
            param.delay_ms = comb_filter_delays[i];
            param.lpf_g = comb_filter_lpf_g[i];
            comb_filters_[i].updateParameters(param);
        }
    }

    void updateAPFParameter() {
        for (int i = 0; i < allpass_filters_.size(); ++i) {
            auto param = allpass_filters_[0].getParameters();
            param.delay_ms = allpass_filters_delays[i];
            param.apf_g = 0.707;
            allpass_filters_[i].updateParameters(param);
        }
    }

    void updateSimpleDelayParameter() {
        auto param = pre_delay_.getParameters();
        param.delay_ms = params_.get(1).getPlainValue();

        pre_delay_.updateParameters(param);
    }

    static constexpr int kNumCombFilter = 6;
    SimpleDelay pre_delay_;
    std::array<CombFilter, kNumCombFilter> comb_filters_;
    std::array<float, kNumCombFilter> comb_filter_delays = {
        29.0f, 31.0f, 37.0f, 41.0f, 43.0f, 47.0f};
    std::array<float, kNumCombFilter> comb_filter_lpf_g = {0.41f, 0.51, 0.61f,
                                                           0.67, 0.83, 0.91};

    std::array<DelayAPF, 1> allpass_filters_;
    std::array<float, 1> allpass_filters_delays = {5};

    AudioProcessorParameters params_;
};
} // namespace libaa

#endif // LIBAA_AA_MOORER_REVERB_PROCESSOR_H
