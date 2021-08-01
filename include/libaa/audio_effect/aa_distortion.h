//
// Created by admin on 2020/7/20.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include <cmath>
#include <memory>

namespace libaa
{
class Distortion : public AudioEffectProcessor
{
public:
    Distortion();

    std::string getName() const override
    {
        return std::string("Distortion");
    }

    void prepareToPlay(double sample_rate, int max_block_size) override;
    void reset() override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float> &buffer) override;

    enum DistortionType
    {
        kHardClipping = 0,
        kSoftClipping,
        kSoftClippingExponential,
        kFullWaveRectifier,
        kHalfWaveRectifier
    };

    float input_gainDb  {0.0f};   // gain in db
    int distortion_type {0};  // index of the type distortion

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

}