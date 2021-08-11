
//
// Created by William.Hua on 2021/7/21.
//

#ifndef LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#define LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#pragma once
#include "libaa/audio_effect/aa_audio_effect_processor.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_audio_block.h"
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

    void process(AudioBufferNew<float>* in_block,
                 AudioBufferNew<float>* out_block);

    void process(AudioBlock* in_block,
                 AudioBlock* out_block);

    const Parameters& getParameters() const;

    int setParameter(int param_index, float normalized_value);

private:
    void applyGain(float* out_buffer, size_t out_size);
    static Parameters buildParameters(float gain_db);

    Parameters params_;
};

}

#endif //LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
