//
// Created by agen on 3/24/22.
//

#ifndef AUDIOEFFECT_AE_DELAY_PROCESSOR_H
#define AUDIOEFFECT_AE_DELAY_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_delay_lines.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/ae_delay_processor_utilities.h"
namespace libaa {
class DelayProcessor : public IAudioProcessor {
public:
    DelayProcessor();

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

private:
    AudioProcessorParameters param_;
    DelayProcessorUtilities utilities;
};
} // namespace libaa

#endif // AUDIOEFFECT_AE_DELAY_PROCESSOR_H
