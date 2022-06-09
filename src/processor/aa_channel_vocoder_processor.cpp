//
// Created by user on 6/9/22.
//

#include "libaa/processor/aa_channel_vocoder_processor.h"
#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_clip_utilities.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/processor/aa_channel_vocoder_utilities.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
ChannelVocoderProcessor::ChannelVocoderProcessor(std::shared_ptr<Clip> carrier_clip,
                                                 std::shared_ptr<ResourcePathFinder> finder)
    : carrier_clip_(std::move(carrier_clip)),
      finder_(std::move(finder)) {

    params_.pushIntParameter("Number Band", 32, 16, 256);
}

std::string ChannelVocoderProcessor::getName() const {
    return {"ChannelVocoder"};
}

void ChannelVocoderProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    PhaseVocoderSetting setting;
    setting.window_size = 2048; // we fix fft parameters here
    setting.hop_size = 1024;
    setting.num_band = params_.get(0).getInt();
    for (auto i = 0u; i < utilities_.size(); ++i) {
        utilities_[i] = std::make_unique<ChannelVocoderUtilities>(setting);
    }
}
void ChannelVocoderProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    bool changed = ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, params_);
    if (changed) {
        updateNumberBand();
    }

    for (auto i = 0u; i < input->context.num_samples; ++i) {
        float carrier_in = carrier_clip_->readSample(0, i + input->context.play_head_sample_index); // use the left channel carrier data

        for (auto c = 0u; c < input->buffer.getNumberChannels(); ++c) {
            float modular_in = input->buffer.getReadPointer(c)[i];
            float y = utilities_[c]->processSample(modular_in, carrier_in);
            output->buffer.getWriterPointer(c)[i] = y;
        }
    }
}
const AudioProcessorParameters *ChannelVocoderProcessor::getParameters() const {
    return &params_;
}
void ChannelVocoderProcessor::setState(uint8_t *state, size_t size) {
    nlohmann::json state_json = JsonUtilities::binaryDataToJson(state, size);
    auto audio_file_uri = state_json["clip"]["audio_file_uri"].get<std::string>();
    auto looping = state_json["clip"]["looping"].get<bool>();
    carrier_clip_ = ClipFactory::buildClip(audio_file_uri, looping, finder_.get());
    if (carrier_clip_ == nullptr) {
        throw std::invalid_argument("cannot open carrier file " + audio_file_uri);
    }

    ProcessorUtilities::updateParametersFromState(state_json, params_);
}
std::vector<uint8_t> ChannelVocoderProcessor::getState() const {
    auto state = ProcessorUtilities::serializeProcessorToJson(this);
    state["clip"] = ClipUtilities::serialize(carrier_clip_.get());
    return JsonUtilities::jsonToBinaryData(state);
}

// ***** for testing *****
const Clip *ChannelVocoderProcessor::getCarrierClip() const {
    return carrier_clip_.get();
}

void ChannelVocoderProcessor::updateNumberBand() {
    auto num_band = params_.get(0).getInt();
    for (auto i = 0u; i < utilities_.size(); ++i) {
        utilities_[i]->updateNumberBand(num_band);
    }
}

} // namespace libaa
