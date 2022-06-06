//
// Created by user on 5/8/22.
//

#include "libaa/processor/aa_source_processor.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
auto createClip(std::list<std::vector<float>> channel_data) {
    AudioBlock block{std::move(channel_data)};
    return std::make_shared<Clip>(block, true);
}

SourceProcessor::SourceProcessor(std::list<std::vector<float>> channel_data)
    : clip_(createClip(std::move(channel_data))) {
}

SourceProcessor::SourceProcessor(std::shared_ptr<Clip> clip,
                                 std::shared_ptr<ResourcePathFinder> finder)
    : clip_(std::move(clip)),
      finder_(std::move(finder)) {
}

std::string SourceProcessor::getName() const {
    return {"Source"};
}
void SourceProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    assert(clip_ != nullptr);
    assert(clip_->getNumberChannels() >= output->buffer.getNumberChannels());

    for (auto c = 0u; c < output->buffer.getNumberChannels(); ++c) {
        float *dest = output->buffer.getWriterPointer(c);
        auto sample_start_index = input->context.play_head_sample_index;
        for (auto i = 0u; i < input->context.num_samples; ++i) {
            dest[i] = clip_->readSample(c, sample_start_index + i);
        }
    }
}
const AudioProcessorParameters *SourceProcessor::getParameters() const {
    return nullptr;
}

const AudioBufferX<float> *SourceProcessor::getChannelData() const {
    return nullptr;
}
void SourceProcessor::setState(uint8_t *state, size_t size) {
    nlohmann::json state_json = JsonUtilities::binaryDataToJson(state, size);
    auto audio_file_uri = state_json["clip"]["audio_file_uri"].get<std::string>();
    auto looping = state_json["clip"]["looping"].get<bool>();
    clip_ = ClipFactory::buildClip(audio_file_uri, looping, finder_.get());
}
std::vector<uint8_t> SourceProcessor::getState() const {
    auto state = ProcessorUtilities::serializeProcessorToJson(this);
    // build clip state
    nlohmann::json clip_state;
    clip_state["audio_file_uri"] = clip_->getAudioFileUri();
    clip_state["looping"] = clip_->isLooping();

    state["clip"] = clip_state;
    return JsonUtilities::jsonToBinaryData(state);
}

} // namespace libaa
