//
// Created by william on 2020/3/10.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include <memory>
namespace libaa {
class Chorus : public AudioEffectProcessor {
public:
    Chorus();

    virtual ~Chorus() = default;

    std::string getName() const override {
        return std::string("chorus");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override;

    void reset() override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    float min_delay = 0.03f;   // minimum lenght of delay line
    float sweep_width = 0.02f; // amount of change from min to max
    float depth = 1.0f;        // mix level of delayed signal [0, 1]
    float lfo_freq = 0.2f;     // lfo frequency(hz)

    int num_voices = 2; // how many voices to use in the chorus [2,5]
    int stereo = 0;     // whether to use stereo chorus

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace libaa
