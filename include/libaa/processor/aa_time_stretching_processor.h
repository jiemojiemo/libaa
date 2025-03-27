//
// Created by user on 6/14/22.
//

#ifndef AUDIOEFFECT_AE_TIME_STRETCHING_PROCESSOR_H
#define AUDIOEFFECT_AE_TIME_STRETCHING_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/offline/aa_time_stretching.h"
#include "libaa/processor/aa_audio_processor.h"

namespace libaa {
class TimeStretchingProcessor : public IAudioProcessor {
public:
    ~TimeStretchingProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;
    const AudioProcessorParameters *getParameters() const override;
    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    void updateUtilitiesParameters(ParameterChanges &param_changes);
    void push(AudioBlock *input);
    void retrieve(AudioBlock *output);
    void buildDataReferFromBlock(AudioBlock *block);
    static constexpr int kMaxChannels = 2;

    TimeStretching utilities_;
    float *data_refer_to_[kMaxChannels];
};
} // namespace libaa

#endif // AUDIOEFFECT_AE_TIME_STRETCHING_PROCESSOR_H
