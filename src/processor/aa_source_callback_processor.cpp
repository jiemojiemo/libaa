//
// Created by user on 5/9/22.
//
#include "libaa/processor/aa_source_callback_processor.h"

namespace libaa {

std::string SourceCallbackProcessor::getName() const {
    return "Source Callback";
}
void SourceCallbackProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    (void)(input);
    if (callback_ != nullptr) {
        callback_(output);
    }
}

const AudioProcessorParameters *SourceCallbackProcessor::getParameters() const {
    return nullptr;
}

void SourceCallbackProcessor::setSourceCallback(SourceCallback callback) {
    callback_ = std::move(callback);
}

SourceCallbackProcessor::SourceCallback SourceCallbackProcessor::getSourceCallback() const {
    return callback_;
}
} // namespace libaa
