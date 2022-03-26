
//
// Created by William.Hua on 2021/7/21.
//

#ifndef LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#define LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
#pragma once
#include "libaa/audio_effect/aa_audio_effect_processor.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_audio_processor.h"
namespace libaa {
class GainProcessor : public IAudioProcessor {
public:
    explicit GainProcessor(float gain_db = 0.0);

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

private:
    void updateGainParameter(AudioBlock *block);
    void applyGain(AudioBlock *block);
    AudioProcessorParameters parameters;
};

} // namespace libaa

#endif // LIBAA_SRC_AUDIO_EFFECT_AA_GAIN_PROCESSOR_H
