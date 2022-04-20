//
// Created by user on 4/19/22.
//

#ifndef LIBAA_AA_REVERB_TANK_UTILITIES_H
#define LIBAA_AA_REVERB_TANK_UTILITIES_H

#pragma once
#include "libaa/dsp/aa_db_utils.h"
#include "libaa/dsp/aa_nested_delay_APF.h"
#include "libaa/dsp/aa_simple_LPF.h"
#include "libaa/dsp/aa_simple_delay.h"
#include "libaa/dsp/aa_two_band_shelving_filter.h"
#include "libaa/processor/aa_audio_processor.h"
#include <array>
#include <iostream>

namespace libaa {
class ReverbTankUtilities {
public:
    enum ParameterIndex {
        kReverbTime = 0,
        kDamping,
        kLowShelfFc,
        kLowShelfGainDB,
        kHighShelfFc,
        kHighShelfGainDB,
        kDensity,
        kPreDelay,
        kWetDB,
        kDryDB,
        kMaxAPFDelay,
        kAPFDelayWeight,
        kMaxFixedDelay,
        kFixedDelayWeight
    };

    ReverbTankUtilities() {
        params.pushFloatParameter("Reverb Time", 0.9f, 0.0f, 1.0f);
        params.pushFloatParameter("Damping", 0.3f, 0.0f, 0.8f);
        params.pushFloatParameter("Low Shelf Fc", 150.0f, 20.0f, 2000.0f);
        params.pushFloatParameter("Low Shelf Gain dB", -20.0f, -35.0f, 35.0f);
        params.pushFloatParameter("High Shelf Fc", 4000.0f, 1000.0f, 5000.0f);
        params.pushFloatParameter("High Shelf Gain dB", -6.0f, -35.0f, 35.0f);
        params.pushChoiceParameter("Density", 0, {"Thick", "Thin"});
        params.pushFloatParameter("PreDelay(ms)", 25.0f, 0.0f, 100.0f);
        params.pushFloatParameter("Wet dB", -12.0f, -60.0f, 20.0f);
        params.pushFloatParameter("Dry dB", 0.0f, -60.0f, 20.0f);
        params.pushFloatParameter("Max APF Delay(ms)", 33.0f, 0.0f, 100.0f);
        params.pushFloatParameter("APF Delay Weight", 0.85f, 0.0f, 1.0f);
        params.pushFloatParameter("Max Fixed Delay(ms)", 81.0f, 0.0f, 100.0f);
        params.pushFloatParameter("Fixed Delay Weight", 1.0f, 0.0f, 1.0f);
    }

    void prepareToPlay(float sample_rate, int max_block_size) {
        (void)(max_block_size);

        const int min_delay_size = 8;
        const int pre_delay_size =
            params.get(kPreDelay).getPlainValue() / 1000.0f * sample_rate +
            min_delay_size;
        pre_delay.prepare(sample_rate, pre_delay_size);

        auto max_apf_delay_ms = params.get(kMaxFixedDelay).getPlainValue();
        auto max_fixed_ms = params.get(kMaxFixedDelay).getPlainValue();
        const int max_delay_size =
            std::max(max_apf_delay_ms, max_fixed_ms) / 1000.0f * sample_rate +
            min_delay_size;
        for (int i = 0; i < kNumBranch; ++i) {
            branch_nested_apfs[i].prepare(sample_rate, max_delay_size);
            branch_delays[i].prepare(sample_rate, max_delay_size);
        }

        for (size_t i = 0; i < shelving_filters.size(); ++i) {
            shelving_filters[i].prepare(sample_rate);
        }

        updatePreDelayParameters();
        updateBranchNestedAPFParameters();
        updateBranchLFPParameters();
        updateBranchDelayParameters();
        updateShelvingFilterParameters();
    }

    void process(AudioBlock *block) {
        updateParameters(block);

        const float kRT = params.get(kReverbTime).getPlainValue();
        bool thick = (params.get(kDensity).getInt() == 0);
        const float dry_scale = dbToScale(params.get(kDryDB).getPlainValue());
        const float wet_scale = dbToScale(params.get(kWetDB).getPlainValue());

        int num_channels = block->buffer.getNumberChannels();
        float *left_channel = block->buffer.getWriterPointer(0);
        float *right_channel =
            num_channels > 1 ? block->buffer.getWriterPointer(1) : left_channel;
        for (size_t i = 0; i < block->buffer.getNumberFrames(); ++i) {
            float global_fb = branch_delays[kNumBranch - 1].readDelay();
            float fb = kRT * global_fb;

            float xn_L = left_channel[i];
            float xn_R = right_channel[i];
            float mono_xn = (xn_L + xn_R) * 0.5f;

            float pre_delay_output = pre_delay.processSample(mono_xn);
            float in = pre_delay_output + fb;

            for (int b = 0; b < kNumBranch; ++b) {
                auto apf_output = branch_nested_apfs[i].processSample(in);
                auto lpf_output = branch_lpfs[i].processSample(apf_output);
                auto delay_output =
                    kRT * branch_delays[i].processSample(lpf_output);
                in = delay_output + pre_delay_output;
            }

            const float w = 0.707;
            float out_L = 0.0f;
            out_L += w * branch_delays[0].readDelayAtPercentage(23.0f);
            out_L -= w * branch_delays[1].readDelayAtPercentage(41.0f);
            out_L += w * branch_delays[2].readDelayAtPercentage(59.0f);
            out_L -= w * branch_delays[3].readDelayAtPercentage(73.0f);

            float out_R = 0.0f;
            out_R -= w * branch_delays[0].readDelayAtPercentage(29.0f);
            out_R += w * branch_delays[1].readDelayAtPercentage(43.0f);
            out_R -= w * branch_delays[2].readDelayAtPercentage(61.0f);
            out_R += w * branch_delays[3].readDelayAtPercentage(79.0f);

            if (thick) {
                out_L += w * branch_delays[0].readDelayAtPercentage(31.0f);
                out_L -= w * branch_delays[1].readDelayAtPercentage(47.0f);
                out_L += w * branch_delays[2].readDelayAtPercentage(67.0f);
                out_L -= w * branch_delays[3].readDelayAtPercentage(83.0f);

                out_R -= w * branch_delays[0].readDelayAtPercentage(37.0f);
                out_R += w * branch_delays[1].readDelayAtPercentage(53.0f);
                out_R -= w * branch_delays[2].readDelayAtPercentage(71.0f);
                out_R += w * branch_delays[3].readDelayAtPercentage(89.0f);
            }

            float shelving_out_L = shelving_filters[0].processSample(out_L);
            float shelving_out_R = shelving_filters[1].processSample(out_R);

            if (num_channels == 1) {
                left_channel[i] =
                    dry_scale * xn_L +
                    wet_scale * 0.5f * (shelving_out_L + shelving_out_R);
            } else {
                left_channel[i] = dry_scale * xn_L + wet_scale * shelving_out_L;
                right_channel[i] =
                    dry_scale * xn_R + wet_scale * shelving_out_R;
            }
        }
    }

    void updatePreDelayParameters() {
        auto pre_delay_ms = params.get(kPreDelay).getPlainValue();
        pre_delay.updateParameters({pre_delay_ms});
    }

    void updateBranchDelayParameters() {
        auto max_fix_delay =
            params.get(ReverbTankUtilities::kMaxFixedDelay).getPlainValue();
        auto weight =
            params.get(ReverbTankUtilities::kFixedDelayWeight).getPlainValue();

        SimpleDelay::SimpleDelayParameters parameters;
        for (int i = 0; i < kNumBranch; ++i) {
            parameters.delay_ms =
                max_fix_delay * weight * fixed_delay_weight[i];
            branch_delays[i].updateParameters(parameters);
        }
    }

    void updateShelvingFilterParameters() {
        TwoBandShelvingFilter::TwoBandShelvingFilterParameters parameters;
        parameters.low_shelf_fc =
            params.get(ReverbTankUtilities::kLowShelfFc).getPlainValue();
        parameters.low_shelf_boost_db =
            params.get(ReverbTankUtilities::kLowShelfGainDB).getPlainValue();
        parameters.high_shelf_fc =
            params.get(ReverbTankUtilities::kHighShelfFc).getPlainValue();
        parameters.high_shelf_boost_db =
            params.get(ReverbTankUtilities::kHighShelfGainDB).getPlainValue();

        for (size_t i = 0; i < shelving_filters.size(); ++i) {
            shelving_filters[i].updateParameters(parameters);
        }
    }

    void updateBranchNestedAPFParameters() {
        auto max_apf_delay =
            params.get(ReverbTankUtilities::kMaxAPFDelay).getPlainValue();
        auto weight =
            params.get(ReverbTankUtilities::kAPFDelayWeight).getPlainValue();

        NestedDelayAPF::NestedDelayAPFParameters parameters;
        parameters.enable_LFO = true;
        parameters.lfo_depth = 1.0f;
        parameters.lfo_max_modulation_ms = 0.3;
        parameters.apf_g = 0.5;
        parameters.inner_apf_g = -0.5;

        int m = 0;
        for (int i = 0; i < kNumBranch; ++i) {
            parameters.delay_ms =
                max_apf_delay * weight * apf_delay_weight[m++];
            parameters.inner_apf_delay_ms =
                max_apf_delay * weight * apf_delay_weight[m++];
            parameters.lfo_rate_hz = apf_lfo_hz[i];

            branch_nested_apfs[i].updateParameters(parameters);
        }
    }

    void updateBranchLFPParameters() {
        SimpleLPF::SimpleLPFParameters parameters{};
        parameters.lpf_g = params.get(kDamping).getPlainValue();
        for (int i = 0; i < kNumBranch; ++i) {
            branch_lpfs[i].updateParameters(parameters);
        }
    }

    void updateParameters(AudioBlock *block) {
        if (block->param_changes.getNumParameters() != 0) {
            ParameterChangePoint point{0, 0, 0};
            for (size_t i = 0; i < params.size(); ++i) {
                if (block->param_changes.pop(i, point)) {
                    params.get(i).setNormalizedValue(point.normalized_value);
                    switch (i) {
                    case kPreDelay:
                        updatePreDelayParameters();
                        break;
                    case kMaxFixedDelay:
                    case kFixedDelayWeight:
                        updateBranchDelayParameters();
                        break;
                    case kMaxAPFDelay:
                    case kAPFDelayWeight:
                        updateBranchNestedAPFParameters();
                        break;
                    case kDamping:
                        updateBranchLFPParameters();
                        break;
                    case kLowShelfFc:
                    case kLowShelfGainDB:
                    case kHighShelfFc:
                    case kHighShelfGainDB:
                        updateShelvingFilterParameters();
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    static constexpr int kNumBranch = 4;
    static constexpr int kMaxChannels = 2;
    AudioProcessorParameters params;

    SimpleDelay pre_delay;
    std::array<NestedDelayAPF, kNumBranch> branch_nested_apfs;
    std::array<SimpleLPF, kNumBranch> branch_lpfs;
    std::array<SimpleDelay, kNumBranch> branch_delays;
    std::array<TwoBandShelvingFilter, kMaxChannels> shelving_filters;

    float apf_delay_weight[kNumBranch * 2] = {0.317, 0.873, 0.477, 0.291,
                                              0.993, 0.757, 0.179, 0.575};
    float apf_lfo_hz[kNumBranch] = {0.15, 0.33, 0.57, 0.73};
    float fixed_delay_weight[kNumBranch] = {1.0, 0.873, 0.707, 0.667};

    int processed_index = 0;
};
} // namespace libaa

#endif // LIBAA_AA_REVERB_TANK_UTILITIES_H
