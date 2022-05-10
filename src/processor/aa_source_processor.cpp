//
// Created by user on 5/8/22.
//

#include "libaa/processor/aa_source_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
SourceProcessor::SourceProcessor()
    : channel_data_(std::make_unique<AudioBufferX<float>>()) {
}

SourceProcessor::SourceProcessor(std::list<std::vector<float>> channel_data)
    : channel_data_(std::make_unique<AudioBufferX<float>>(std::move(channel_data))) {
}

std::string SourceProcessor::getName() const {
    return {"Source Processor"};
}
void SourceProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    (void)(input);
    assert(output->buffer.getNumberChannels() <= channel_data_->getNumberChannels());
    assert(output->buffer.getNumberFrames() <= channel_data_->getNumberFrames());

    output->buffer.copyFrom(channel_data_.get());
}
const AudioProcessorParameters *SourceProcessor::getParameters() const {
    return nullptr;
}

const AudioBufferX<float> *SourceProcessor::getChannelData() const {
    return channel_data_.get();
}
void SourceProcessor::setState(uint8_t *state, size_t size) {
    (void)(state);
    (void)(size);
}
std::vector<uint8_t> SourceProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

} // namespace libaa
