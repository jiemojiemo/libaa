//
// Created by user on 5/9/22.
//
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
SourceCallbackProcessor::SourceCallbackProcessor()
    : last_param_change_(8) {
}

std::string SourceCallbackProcessor::getName() const {
    return "Source Callback";
}
void SourceCallbackProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    (void)(input);
    if (callback_ != nullptr) {
        callback_(output);
    }

    // pop parameter change to output block
    ParameterChangePoint point{};
    for (; last_param_change_.pop(point);) {
        output->param_changes.push(point.index, point);
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

void SourceCallbackProcessor::pushParameterChange(int param_index, float norm_val) {
    last_param_change_.push({param_index, 0, norm_val});
}

void SourceCallbackProcessor::setState(uint8_t *state, size_t size) {
    (void)(state);
    (void)(size);
}
std::vector<uint8_t> SourceCallbackProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}
} // namespace libaa