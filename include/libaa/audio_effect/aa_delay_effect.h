//
// Created by william on 2020/2/24.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include "libaa/core/aa_delay_line.h"
#include <memory>
namespace libaa {
class DelayEffect : public AudioEffectProcessor {
public:
    DelayEffect();

    std::string getName() const override {
        return "delay";
    }

    void prepareToPlay(double sample_rate, int max_block_size) override;

    void reset() override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    float delay_length_ = 1;
    float feedback_ = 0.8;
    float dry_mix_ = 1.0;
    float wet_mix_ = 0.2;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace libaa
