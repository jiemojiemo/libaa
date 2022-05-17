//
// Created by user on 5/9/22.
//

#ifndef LIBAA_AA_SOURCE_CALLBACK_PROCESSOR_H
#define LIBAA_AA_SOURCE_CALLBACK_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_audio_processor.h"
namespace libaa {
class SourceCallbackProcessor : public IAudioProcessor {
public:
    SourceCallbackProcessor();
    ~SourceCallbackProcessor() override = default;

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    using SourceCallback = std::function<void(AudioBlock *output_block)>;
    void setSourceCallback(SourceCallback callback);
    SourceCallback getSourceCallback() const;

    void pushParameterChange(int param_index, float norm_val);

    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    SourceCallback callback_{nullptr};
    ParameterChange last_param_change_;
};
} // namespace libaa

#endif // LIBAA_AA_SOURCE_CALLBACK_PROCESSOR_H
