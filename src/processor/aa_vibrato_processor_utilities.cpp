//
// Created by William on 3/26/22.
//

#include "libaa/processor/aa_vibrato_processor_utilities.h"
#include <iostream>
namespace libaa {
void VibratorProcessorUtility::prepare(float sample_rate, int max_block_size) {
    (void)(max_block_size);
    const int kMaxDelaySamples = static_cast<int>(2 * sample_rate);
    delay_utilities_.prepare(kMaxDelaySamples);

    lfo_.waveform = GeneratorWaveform::kSin;
    lfo_.prepare(sample_rate);
}
void VibratorProcessorUtility::process(AudioBufferX<float> *buffer, float depth,
                                       float rate) {
    // update lfo rate
    if (!approximatelyEqual(rate, lfo_.frequency_hz, DBL_EPSILON)) {
        lfo_.updateFrequencyHz(rate);
    }

    auto delay_generator = [&, this]() {
        auto lfo_output = lfo_.renderAudioOutput();
        auto delay_ms = doBipolarModulation(lfo_output.normal_output * depth);
        return delay_ms * lfo_.getSampleRate() / 1000.0f;
    };

    delay_utilities_.processWithDelayGenerator(buffer, delay_generator,
                                               kFeedback, kDry, kWet);
}

float VibratorProcessorUtility::doBipolarModulation(float sin_value) {
    return sin_value * kDelayHalfRange + kDelayMiddlePoint;
}

} // namespace libaa