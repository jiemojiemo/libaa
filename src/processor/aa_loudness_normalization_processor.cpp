//
// Created by user on 11/30/22.
//
#include "libaa/processor/aa_loudness_normalization_processor.h"
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace {
enum LoudnormParameterIndex {
    kTargetLoudness = 0,
    kTargetTruePeak = 1,
    kLiveMode = 2,
    kMeasuredLoudness = 3,
    kMeasuredTruePeak = 4
};

} // namespace

namespace libaa {
LoudnessNormalizationProcessor::LoudnessNormalizationProcessor() {
    params_.pushFloatParameter("Target Loudness LUFS", -16, -70, 0);
    params_.pushFloatParameter("Target True Peak dB", -1, -9, 0);
    params_.pushBoolParameter("Live Mode", true);
    params_.pushFloatParameter("Measured Loudness LUFS", 0, -99, 0);
    params_.pushFloatParameter("Measured True Peak dB", 0, -99, 0);
}

void LoudnessNormalizationProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    limiter_utilities_.prepare(sample_rate, kNumChannels);
    meter_.prepare(kNumChannels, sample_rate, max_block_size);
    data_refer_to_.resize(kNumChannels);

    // configure moving average
    moving_average_.setParameter("size", 7);
    moving_average_.configure();
}

std::string LoudnessNormalizationProcessor::getName() const {
    return {"LoudnessNormalization"};
}
void LoudnessNormalizationProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, params_);

    bool is_live_mode = params_.get(kLiveMode).getBool();
    float target_lufs = params_.get(kTargetLoudness).getPlainValue();
    float target_peak = params_.get(kTargetTruePeak).getPlainValue();

    if (!is_live_mode) {
        offlineLoudnessProcessing(output, target_lufs, target_peak);
    } else {
        liveLoudnessProcessing(output, target_lufs, target_peak);
    }
}
const AudioProcessorParameters *LoudnessNormalizationProcessor::getParameters() const {
    return &params_;
}
void LoudnessNormalizationProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, params_);
}
std::vector<uint8_t> LoudnessNormalizationProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

void LoudnessNormalizationProcessor::offlineLoudnessProcessing(AudioBlock *output, float target_lufs, float target_peak) {
    auto input_loud = params_.get(kMeasuredLoudness).getPlainValue();
    auto loud_gain_db = target_lufs - input_loud;

    auto limiter_params = limiter_utilities_.getParameters();
    limiter_params.threshold_db = target_peak;
    limiter_utilities_.updateParameters(limiter_params, loud_gain_db);

    limiter_utilities_.processBlock(output);
}

void LoudnessNormalizationProcessor::liveLoudnessProcessing(AudioBlock *output, float target_lufs, float target_peak) {
    int num_channel = std::min(size_t(kNumChannels), output->buffer.getNumberChannels());
    prepareBuffersForProcessing(output, num_channel);
    bool ok = meter_.process(data_refer_to_.data(), num_channel, output->buffer.getNumberFrames());
    auto measured_loudness = meter_.momentary;
    if (ok && measured_loudness > MIR::Ebur128Utilities::kSilenceThreshold) {
        float gain_db = target_lufs - measured_loudness;
        float smooth_db = calcSmoothGainDb(gain_db);
        auto limiter_params = limiter_utilities_.getParameters();
        limiter_params.threshold_db = target_peak;
        limiter_utilities_.updateParameters(limiter_params, smooth_db);
        limiter_utilities_.processBlock(output);
    }
}

void LoudnessNormalizationProcessor::prepareBuffersForProcessing(AudioBlock *output, int num_channels) {
    data_refer_to_[0] = output->buffer.getWriterPointer(0);
    if (num_channels == 1) {
        data_refer_to_[1] = output->buffer.getWriterPointer(0);
    } else if (num_channels == 2) {
        data_refer_to_[1] = output->buffer.getWriterPointer(1);
    }
}

float LoudnessNormalizationProcessor::calcSmoothGainDb(float in_db) {
    float out = 0.0f;
    moving_average_.input("signal").set(in_db);
    moving_average_.output("signal").set(out);

    moving_average_.compute();

    return out;
}

} // namespace libaa