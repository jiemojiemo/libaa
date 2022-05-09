//
// Created by user on 5/8/22.
//

#ifndef LIBAA_AA_SOURCE_PROCESSOR_H
#define LIBAA_AA_SOURCE_PROCESSOR_H

#pragma once
#include "libaa/processor/aa_audio_processor.h"
#include <cassert>
namespace libaa {
class SourceProcessor : public IAudioProcessor {
public:
    SourceProcessor();

    SourceProcessor(std::list<std::vector<float>> channel_data);

    ~SourceProcessor() override = default;

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    const AudioBufferX<float> *getChannelData() const;

    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    std::unique_ptr<AudioBufferX<float>> channel_data_{nullptr};
};
} // namespace libaa

#endif // LIBAA_AA_SOURCE_PROCESSOR_H
