
//
// Created by William.Hua on 2020/12/8.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include "aa_biquad_impl.h"

namespace libaa
{
class BiquadFilter : public AudioEffectProcessor
{
public:
    std::string getName() const override {
        return std::string("BiquadFilter");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override {
    }
    void reset() override {
    }
    void releaseResources() override {
    }
    void processBlock(AudioBuffer<float> &buffer) override;

    void setCoefficients(const FilterCoeffs& coff);
private:
    constexpr static size_t kDefaultNumChannels{2};
    std::vector<Biquad> biquad_filters_{kDefaultNumChannels};
};
}
