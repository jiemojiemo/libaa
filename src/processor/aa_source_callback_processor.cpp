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
void SourceCallbackProcessor::setState(uint8_t *state, size_t size) {
    (void)(state);
    (void)(size);
}
std::vector<uint8_t> SourceCallbackProcessor::getState() const {
    return std::vector<uint8_t>();
}
} // namespace libaa
