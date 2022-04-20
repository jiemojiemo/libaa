//
// Created by user on 4/19/22.
//

#ifndef LIBAA_AA_REVERB_TANK_PROCESSOR_H
#define LIBAA_AA_REVERB_TANK_PROCESSOR_H

#pragma once
#include "libaa/processor/aa_reverb_tank_utilities.h"
namespace libaa {
class ReverbTankProcessor : public IAudioProcessor {
public:
    ReverbTankProcessor() = default;

    ~ReverbTankProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    ReverbTankUtilities utilities_;
};
} // namespace libaa

#endif // LIBAA_AA_REVERB_TANK_PROCESSOR_H
