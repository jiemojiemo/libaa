//
// Created by agen on 4/11/22.
//

#ifndef AUDIOEFFECT_AE_PITCH_SHIFTER_PROCESSOR_H
#define AUDIOEFFECT_AE_PITCH_SHIFTER_PROCESSOR_H

#pragma once
#include "libaa/processor/aa_audio_processor.h"

namespace libaa {
class PitchShiftProcessor : public IAudioProcessor {
public:
    PitchShiftProcessor();
    ~PitchShiftProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    int getLatencySamples() const noexcept override;

    void *getPitchShifterImpl() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa

#endif // AUDIOEFFECT_AE_PITCH_SHIFTER_PROCESSOR_H
