//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AA_CHANNEL_VOCODER_PROCESSOR_H
#define LIBAA_AA_CHANNEL_VOCODER_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_resource_path_finder.h"
#include "libaa/processor/aa_audio_processor.h"
#include <array>
#include <memory>
namespace libaa {
class Clip;
class ChannelVocoderUtilities;
class ChannelVocoderProcessor : public IAudioProcessor {
public:
    ChannelVocoderProcessor(std::shared_ptr<Clip> carrier_clip,
                            std::shared_ptr<ResourcePathFinder> finder = nullptr);
    ~ChannelVocoderProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    void setState(uint8_t *state, size_t size) override;

    std::vector<uint8_t> getState() const override;

    // ***** for testing *****
    const Clip *getCarrierClip() const;

private:
    void updateNumberBand();
    AudioProcessorParameters params_;
    std::shared_ptr<Clip> carrier_clip_{nullptr};
    std::shared_ptr<ResourcePathFinder> finder_{nullptr};
    std::array<std::shared_ptr<ChannelVocoderUtilities>, 2> utilities_;
};
} // namespace libaa

#endif // LIBAA_AA_CHANNEL_VOCODER_PROCESSOR_H
