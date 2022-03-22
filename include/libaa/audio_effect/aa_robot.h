//
// Created by admin on 2020/8/5.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include "libaa/dsp/aa_window.h"
#include <memory>
namespace libaa {
class Robotisation : public AudioEffectProcessor {
public:
    Robotisation();

    std::string getName() const override { return std::string("Robot"); }
    void prepareToPlay(double sample_rate, int max_block_size) override;

    void reset() override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    // parameters
    size_t fft_size = {2048};
    size_t hop_size = {512};
    WindowType window_type = {WindowType::kHann};

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa
