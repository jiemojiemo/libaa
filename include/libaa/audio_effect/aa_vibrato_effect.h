//
// Created by william on 2020/2/28.
//

#pragma once

#include "aa_audio_effect_processor.h"
#include "libaa/audio_basics/aa_delay_line.h"
#include "libaa/dsp/aa_lfo.h"
#include <memory>

namespace libaa {
class VibratoEffect : public AudioEffectProcessor {
public:
    VibratoEffect();
    std::string getName() const override { return std::string("vibrato"); }
    void prepareToPlay(double sample_rate, int max_block_size) override;

    void reset() override {}

    void releaseResources() override {}

    void processBlock(AudioBuffer<float> &buffer) override;

    float lfo_freq = 4.0f;
    float sweep_width = 0.01f;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa
