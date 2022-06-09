//
// Created by William on 3/26/22.
//

#ifndef AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_UTILITIES_H
#define AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_UTILITIES_H

#pragma once
#include "libaa/audio_generator/aa_lfo_generator.h"
#include "libaa/core/aa_audio_buffer.h"
#include "libaa/processor/aa_delay_processor_utilities.h"
#include <cfloat>
namespace libaa {
class VibratorProcessorUtility {
public:
    void prepare(float sample_rate, int max_block_size);

    void process(AudioBufferX<float> *buffer, float depth, float rate);

private:
    static float doBipolarModulation(float sin_value);

    static constexpr float kMinDelayMS = 0;
    static constexpr float kMaxDelayMS = 10;
    static constexpr float kDelayRange = kMaxDelayMS - kMinDelayMS;
    static constexpr float kDelayHalfRange = kDelayRange / 2;
    static constexpr float kDelayMiddlePoint = kDelayHalfRange + kMinDelayMS;
    static constexpr float kFeedback = 0;
    static constexpr float kDry = 0;
    static constexpr float kWet = 1.0f;

private:
    LFOGenerator lfo_;
    DelayProcessorUtilities delay_utilities_;
};

} // namespace libaa

#endif // AUDIOEFFECT_AE_VIBRATOR_PROCESSOR_UTILITIES_H
