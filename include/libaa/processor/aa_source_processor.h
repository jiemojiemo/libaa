//
// Created by user on 5/8/22.
//

#ifndef LIBAA_AA_SOURCE_PROCESSOR_H
#define LIBAA_AA_SOURCE_PROCESSOR_H

#pragma once
#include "libaa/core/aa_resource_path_finder.h"
#include "libaa/processor/aa_audio_processor.h"
#include <cassert>
namespace libaa {
class Clip;
class SourceProcessor : public IAudioProcessor {
public:
    SourceProcessor() = default;

    SourceProcessor(std::list<std::vector<float>> channel_data);

    SourceProcessor(std::shared_ptr<Clip> clip,
                    std::shared_ptr<ResourcePathFinder> finder = nullptr);

    ~SourceProcessor() override = default;

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    const AudioBufferX<float> *getChannelData() const;

    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    std::shared_ptr<Clip> clip_{nullptr};
    std::shared_ptr<ResourcePathFinder> finder_{nullptr};
};
} // namespace libaa

#endif // LIBAA_AA_SOURCE_PROCESSOR_H
