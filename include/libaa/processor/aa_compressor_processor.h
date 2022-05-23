//
// Created by user on 5/23/22.
//

#ifndef LIBAA_AA_COMPRESSOR_PROCESSOR_H
#define LIBAA_AA_COMPRESSOR_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_dynamic_processor_utilities.h"
#include <array>
namespace libaa {
class Compressor : public IAudioProcessor {
public:
    Compressor();
    ~Compressor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    void setState(uint8_t *state, size_t size) override;

    std::vector<uint8_t> getState() const override;

    //** just for testing **
    const std::array<DynamicProcessorUtilities, 2> &getUtilities() const;

private:
    void updateDynamicProcessorUtilitiesParameters();

    AudioProcessorParameters params_;
    std::array<DynamicProcessorUtilities, 2> utilities_;
};
} // namespace AudioEffect

#endif // LIBAA_AA_COMPRESSOR_PROCESSOR_H
