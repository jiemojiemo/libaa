//
// Created by user on 5/23/22.
//

#include "libaa/processor/aa_compressor_processor.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
enum CompressorParameterIndex {
    kThreshold = 0,
    kKneeWidth = 1,
    kRatio = 2,
    kAttackTime = 3,
    kReleaseTime = 4,
    kOutputGain = 5,
    kSoftKnee = 6
};
Compressor::Compressor() {
    params_.pushFloatParameter("Threshold", -10, -40, 0);
    params_.pushFloatParameter("Knee Width", 5, 0, 20);
    params_.pushFloatParameter("Ratio", 1, 1, 20);
    params_.pushFloatParameter("Attack Time(ms)", 5, 1, 100);
    params_.pushFloatParameter("Release Time(ms)", 500, 1, 5000);
    params_.pushFloatParameter("Output Gain dB", 0, -20, 20);
    params_.pushBoolParameter("Soft Knee", true);
}
std::string Compressor::getName() const {
    return {"Compressor"};
}
void Compressor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    for (auto &n : utilities_) {
        n.prepare(sample_rate);
    }

    // update parameter after prepare
    updateDynamicProcessorUtilitiesParameters();
}
void Compressor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    bool changed = ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, params_);
    if (changed) {
        updateDynamicProcessorUtilitiesParameters();
    }

    for (auto c = 0u; c < output->buffer.getNumberChannels(); ++c) {
        DynamicProcessorUtilities &u = utilities_[c];
        float *channel_data = output->buffer.getWriterPointer(c);
        for (auto i = 0u; i < output->buffer.getNumberFrames(); ++i) {
            float in = channel_data[i];
            channel_data[i] = u.process(in);
        }
    }
}
const AudioProcessorParameters *Compressor::getParameters() const {
    return &params_;
}
void Compressor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, params_);
}
std::vector<uint8_t> Compressor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

void Compressor::updateDynamicProcessorUtilitiesParameters() {
    auto dynamic_params = utilities_[0].getParameters();
    dynamic_params.threshold_db = params_.get(kThreshold).getPlainValue();
    dynamic_params.knee_width = params_.get(kKneeWidth).getPlainValue();
    dynamic_params.ratio = params_.get(kRatio).getPlainValue();
    dynamic_params.attack_time_ms = params_.get(kAttackTime).getPlainValue();
    dynamic_params.release_time_ms = params_.get(kReleaseTime).getPlainValue();
    dynamic_params.output_gain_db = params_.get(kOutputGain).getPlainValue();
    dynamic_params.soft_knee = params_.get(kSoftKnee).getBool();

    for (auto &u : utilities_) {
        u.updateParameters(dynamic_params);
    }
}

//** just for testing **
const std::array<DynamicProcessorUtilities, 2> &Compressor::getUtilities() const {
    return utilities_;
}
} // namespace libaa