//
// Created by william on 2020/3/3.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include <memory>
namespace libaa {
class Flanger : public AudioEffectProcessor {
public:
    Flanger();
    std::string getName() const override {
        return std::string("flanger");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override;
    void reset() override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float> &buffer) override;

    float lfo_freq = 1;        // frequency fo the lfo
    float sweep_width = 0.003; // width of the lfo samples
    float depth = 1.0;    // amount o delayed signal mixed with original (0,1)
    float feedback = 0.3; // amount of feedback (0, 1)
    float min_delay = 0.001; // minimum length of delay line in seconds
    int stereo = 1;          // whether to use stereo flanging

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa
