
//
// Created by William.Hua on 2021/7/21.
//

#ifndef LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#define LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#pragma once
#include "libaa/audio_effect/aa_audio_effect_processor.h"
#include "libaa/core/aa_audio_processor_parameters.h"
namespace libaa
{
class GainProcessor : public AudioEffectProcessor
{
public:
    GainProcessor(float gain_db = 0);
    std::string getName() const override;
    void prepareToPlay(double sample_rate, int max_block_size) override;
    void reset() override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float> &buffer) override;

    void process(float* in_buffer, size_t in_size,
                 float* out_buffer, size_t out_size);

    const Parameters& getParameters() const;

private:
    void applyGain(float* out_buffer, size_t out_size);
    static Parameters buildParameters(float gain_db);

    Parameters params_;
};

}

#endif //LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
