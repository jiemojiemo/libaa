//
// Created by william on 2020/6/23.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include <cmath>
#include <memory>
namespace libaa
{
class Compressor : public AudioEffectProcessor
{
public:
    Compressor();

    ~Compressor() override = default;

    std::string getName() const override {
        return std::string("Compressor");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override;

    void reset() override;

    void releaseResources() override;

    void processBlock(AudioBuffer<float> &buffer) override;

    void setThreshold(float v);
    void setAttackTime(float time_ms);
    void setReleaseTime(float time_ms);
    void setRatio(float r);
    void setMakeupGain(float g);

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
