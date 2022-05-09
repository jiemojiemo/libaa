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
    ~SourceCallbackProcessor() override = default;

    std::string getName() const override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    using SourceCallback = std::function<void(AudioBlock *output_block)>;
    void setSourceCallback(SourceCallback callback);

    SourceCallback getSourceCallback() const;
private:
    SourceCallback callback_{nullptr};
};
} // namespace libaa

#endif // LIBAA_AA_SOURCE_CALLBACK_PROCESSOR_H
