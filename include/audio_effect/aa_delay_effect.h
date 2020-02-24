//
// Created by william on 2020/2/24.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include "audio_basics/aa_delay_line.h"
namespace libaa
{
class DelayEffect : public AudoEffectProcessor
{
public:
    DelayEffect()= default;

    std::string getName() const override
    {
        return "delay";
    }

    void prepareToPlay(double sample_rate, int samplers_per_block) override;

    void reset() override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    float delay_length_ = 1;
    float feedback_ = 0.8;
    float dry_mix_ = 1.0;
    float wet_mix_ = 0.2;

private:
    std::vector<DelayLine<float>> dlines_;
    std::vector<size_t> delay_length_in_sample_;
};

}
