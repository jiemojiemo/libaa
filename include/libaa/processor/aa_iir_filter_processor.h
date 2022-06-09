//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AA_IIR_FILTER_PROCESSOR_H
#define LIBAA_AA_IIR_FILTER_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/dsp/aa_iir_filter.h"
#include "libaa/processor/aa_audio_processor.h"

namespace libaa {
class IIRFilterProcessor : public IAudioProcessor {
public:
    IIRFilterProcessor();

    ~IIRFilterProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    void setState(uint8_t *state, size_t size) override;

    std::vector<uint8_t> getState() const override;

private:
    void updateIIRFilterParameters();

    AudioProcessorParameters params_;
    std::array<IIRFilter, 2> filters_;
};
} // namespace libaa

#endif // LIBAA_AA_IIR_FILTER_PROCESSOR_H
