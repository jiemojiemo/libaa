//
// Created by user on 5/2/22.
//

#ifndef LIBAA_AA_SCHROEDER_REVERB_PROCESSOR_H
#define LIBAA_AA_SCHROEDER_REVERB_PROCESSOR_H

#pragma once
#include "libaa/dsp/aa_comb_filter.h"
#include "libaa/dsp/aa_delay_APF.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <array>

namespace libaa {
class SchroederReverbProcessor : public IAudioProcessor {
public:
    SchroederReverbProcessor() {
        params_.pushFloatParameter("Reverb Time(ms)", 50.0f, 0.0f, 2000.0f);
        params_.pushFloatParameter("Wet", 1.0f, 0.0f, 1.0f);
        params_.pushFloatParameter("Dry", 0.0f, 0.0f, 1.0f);
    }
    ~SchroederReverbProcessor() override = default;
    std::string getName() const override {
        return {"SchroederReverb"};
    }
    void prepareToPlay(float sample_rate, int max_block_size) override {
        const int max_delay_size = 1.0 * sample_rate;
        for (int i = 0; i < comb_filters_.size(); ++i) {
            comb_filters_[i].prepare(sample_rate, max_delay_size);
        }

        for (int i = 0; i < allpass_filters_.size(); ++i) {
            allpass_filters_[i].prepare(sample_rate, max_delay_size);
        }

        updateCombFilterParameter(params_.get(0).getPlainValue());
        updateAPFParameter();
    }
    void processBlock(AudioBlock *input, AudioBlock *output) override {
        output->buffer.copyFrom(&input->buffer);

        ProcessorUtilities::updateParameterFromParameterChanges(
            input->param_changes, params_);
        updateCombFilterParameter(params_.get(0).getPlainValue());

        auto num_channels = output->buffer.getNumberChannels();
        float *left_channel = output->buffer.getWriterPointer(0);
        float *right_channel = num_channels > 1
                                   ? output->buffer.getWriterPointer(1)
                                   : left_channel;
        float wet = params_.get(1).getPlainValue();
        float dry = params_.get(2).getPlainValue();
        for (int i = 0; i < output->buffer.getNumberFrames(); ++i) {
            float xn_L = left_channel[i];
            float xn_R = right_channel[i];
            float mono_xn = (xn_L + xn_R) * 0.5f;

            float comb_output = 0.0f;
            comb_output += comb_filters_[0].processSample(mono_xn);
            comb_output -= comb_filters_[1].processSample(mono_xn);
            comb_output += comb_filters_[2].processSample(mono_xn);
            comb_output -= comb_filters_[3].processSample(mono_xn);

            float apf_output = comb_output;
            for (auto &apf : allpass_filters_) {
                apf_output = apf.processSample(apf_output);
            }

            float output = mono_xn * dry + wet * apf_output;
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
            param.rt_60_ms = new_rt_60;
            param.delay_ms = comb_filter_delays[i];
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

    AudioProcessorParameters params_;
    std::array<CombFilter, 4> comb_filters_;
    std::array<float, 4> comb_filter_delays = {31.0f, 37.0f, 43.0f, 47.0f};

    std::array<DelayAPF, 2> allpass_filters_;
    std::array<float, 2> allpass_filters_delays = {3, 5};
};
} // namespace libaa

#endif // LIBAA_AA_SCHROEDER_REVERB_PROCESSOR_H
