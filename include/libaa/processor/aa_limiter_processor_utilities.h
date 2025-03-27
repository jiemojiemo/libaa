//
// Created by user on 4/17/23.
//

#ifndef AUDIOEFFECT_AE_LIMITER_PROCESSOR_UTILITIES_H
#define AUDIOEFFECT_AE_LIMITER_PROCESSOR_UTILITIES_H
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_dynamic_processor_utilities.h"
#include <vector>
namespace libaa {
class LimiterProcessorUtilities {
public:
    void prepare(float sample_rate, int num_channels) {
        dynamic_utilities.resize(num_channels);

        for (auto &u : dynamic_utilities) {
            u.prepare(sample_rate);
        }

        initDynamicProcessorParameters();
    }

    void initDynamicProcessorParameters() {
        parameters.ratio = std::numeric_limits<float>::max();
        parameters.knee_width = 10;
        parameters.attack_time_ms = 0.0001f;
        parameters.output_gain_db = 0.0f;
        parameters.soft_knee = true;
        parameters.hard_limiter = false;
        pre_input_gain_db_ = 0.0f;
        input_gain_db_ = 0.0f;
    }

    const DynamicProcessorParameters &getParameters() const {
        return parameters;
    }

    void updateParameters(float threshold, float release_time,
                          float input_gain_db) {
        if (dynamic_utilities.empty()) {
            return;
        }

        parameters.threshold_db = threshold;
        parameters.release_time_ms = release_time;

        updateParameters(parameters, input_gain_db);
    }

    void updateParameters(const DynamicProcessorParameters &params,
                          float input_gain_db) {
        for (auto &u : dynamic_utilities) {
            u.updateParameters(params);
        }
        pre_input_gain_db_ = input_gain_db_;
        input_gain_db_ = input_gain_db;
    }

    void processBlock(AudioBlock *io_buffer) {
        // apply gain ramp, avoid clipping
        io_buffer->buffer.applyGainRamp(0, io_buffer->context.num_samples, DbUtils::dbToScale(pre_input_gain_db_), DbUtils::dbToScale(input_gain_db_));

        for (auto c = 0u; c < io_buffer->buffer.getNumberChannels(); ++c) {
            DynamicProcessorUtilities &u = dynamic_utilities[c];
            float *channel_data = io_buffer->buffer.getWriterPointer(c);
            for (auto i = 0u; i < io_buffer->context.num_samples; ++i) {
                channel_data[i] = u.process(channel_data[i]);
            }
        }
    }

    std::vector<DynamicProcessorUtilities> dynamic_utilities;
    float pre_input_gain_db_{0.0f};
    float input_gain_db_{0.0f};
    DynamicProcessorParameters parameters;
};
} // namespace AudioEffect

#endif // AUDIOEFFECT_AE_LIMITER_PROCESSOR_UTILITIES_H
