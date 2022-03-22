
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/audio_effect/aa_gain_processor.h"
#include "libaa/dsp/aa_db_utils.h"
#include <cmath>
namespace libaa {
GainProcessor::GainProcessor(float gain_db)
    : params_(buildParameters(gain_db)) {}

std::string GainProcessor::getName() const { return std::string("Gain"); }

void GainProcessor::prepareToPlay(double sample_rate, int max_block_size) {
    (void)(sample_rate);
    (void)(max_block_size);
}

void GainProcessor::reset() {}
void GainProcessor::releaseResources() {}
void GainProcessor::processBlock(AudioBuffer<float> &buffer) { (void)(buffer); }

const Parameters &GainProcessor::getParameters() const { return params_; }

void GainProcessor::applyGain(float *out_buffer, size_t out_size) {
    float gain_db = params_.get(0).getPlainValue();
    if (gain_db != 0.0f) {
        float gain_scale = dbToScale(gain_db);
        for (size_t i = 0; i < out_size; ++i) {
            out_buffer[i] *= gain_scale;
        }
    }
}

Parameters GainProcessor::buildParameters(float gain_db) {
    Parameters params;

    params.pushFloatParameter("Gain dB", gain_db, -75, 35);

    return params;
}

void GainProcessor::process(AudioBlock *in_block, AudioBlock *out_block) {
    out_block->audio_buffer.copyFrom(&in_block->audio_buffer);

    if (in_block->param_changes.empty()) {
        applyStaticGain(out_block);
    } else {
        applyDynamicGain(in_block->param_changes, out_block);
    }
}

void GainProcessor::applyStaticGain(AudioBlock *out_block) {

    for (auto c = 0u; c < out_block->audio_buffer.getNumberChannels(); ++c) {
        float *channel_data = out_block->audio_buffer.getWriterPointer(c);
        applyGain(channel_data, out_block->audio_buffer.getNumberFrames());
    }
}

auto getRampValue = [](size_t current_sample, size_t num_samples,
                       float start_value, float end_value) {
    int num_step = std::min(int(num_samples - 1), 1);

    float step = (end_value - start_value) / num_step;
    return start_value + step * current_sample;
};

void GainProcessor::applyDynamicGain(const ParameterChanges &param_changes,
                                     AudioBlock *out_block) {
    const size_t num_samples = out_block->audio_buffer.getNumberFrames();
    const size_t num_channels = out_block->audio_buffer.getNumberChannels();

    AudioProcessorParameter &gain_p = params_.get(0);
    float normalized_start_gain = gain_p.getNormalizedValue();
    float normalized_end_gain = param_changes.at(0)->back()->normalized_value;

    for (auto i = 0u; i < num_samples; ++i) {
        gain_p.setNormalizedValue(getRampValue(
            i, num_samples, normalized_start_gain, normalized_end_gain));
        float dynamic_gain_scale = dbToScale(gain_p.getPlainValue());

        for (auto c = 0u; c < num_channels; ++c) {
            out_block->audio_buffer.getWriterPointer(c)[i] *=
                dynamic_gain_scale;
        }
    }
}

} // namespace libaa