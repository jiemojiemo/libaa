//
// Created by William on 3/26/22.
//

#ifndef AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_H
#define AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_vibrator_processor_utilities.h"
namespace libaa {
class AudioBlock;
class VibratorProcessor : public IAudioProcessor {
public:
    VibratorProcessor();

    virtual void prepareToPlay(float sample_rate, int max_block_size) override;

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

private:
    AudioProcessorParameters param_;
    VibratorProcessorUtility utility_;
};

} // namespace libaa

#endif // AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_H