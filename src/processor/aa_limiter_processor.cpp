//
// Created by user on 5/23/22.
//

#include "libaa/processor/aa_limiter_processor.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
enum CompressorParameterIndex {
    kThreshold = 0,
    kInputGain = 1,
    kReleaseTime = 2
};

LimiterProcessor::LimiterProcessor() {
    params_.pushFloatParameter("Threshold", -10, -40, 0);
    params_.pushFloatParameter("Input Gain dB", 0, 0, 35);
    params_.pushFloatParameter("Release Time(ms)", 500, 1, 5000);
}
std::string LimiterProcessor::getName() const {
    return {"Limiter"};
}
void LimiterProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    for (auto &n : utilities_) {
        n.prepare(sample_rate);
    }
    updateDynamicProcessorUtilitiesParameters();
}
void LimiterProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    bool changed = ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, params_);
    if (changed) {
        updateDynamicProcessorUtilitiesParameters();
    }

    float input_gain = DbUtils::dbToScale(params_.get(kInputGain).getPlainValue());
    for (auto c = 0u; c < output->buffer.getNumberChannels(); ++c) {
        DynamicProcessorUtilities &u = utilities_[c];
        float *channel_data = output->buffer.getWriterPointer(c);
        for (auto i = 0u; i < output->buffer.getNumberFrames(); ++i) {
            float in = channel_data[i] * input_gain;
            channel_data[i] = u.process(in);
        }
    }
}
const AudioProcessorParameters *LimiterProcessor::getParameters() const {
    return &params_;
}
void LimiterProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, params_);
}
std::vector<uint8_t> LimiterProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

//** just for testing **
const std::array<DynamicProcessorUtilities, 2> &LimiterProcessor::getUtilities() const {
    return utilities_;
}
void LimiterProcessor::updateDynamicProcessorUtilitiesParameters() {
    auto dynamic_params = utilities_[0].getParameters();
    dynamic_params.threshold_db = params_.get(kThreshold).getPlainValue();
    dynamic_params.knee_width = 10;
    dynamic_params.ratio = std::numeric_limits<float>::max();
    dynamic_params.attack_time_ms = 1;
    dynamic_params.release_time_ms = params_.get(kReleaseTime).getPlainValue();
    dynamic_params.output_gain_db = 0;
    dynamic_params.soft_knee = true;
    dynamic_params.hard_limiter = true;

    for (auto &u : utilities_) {
        u.updateParameters(dynamic_params);
    }
}
} // namespace libaa
