//
// Created by William on 3/26/22.
//

#ifndef AUDIOEFFECT_AE_DELAY_PROCESSOR_UTILITIES_H
#define AUDIOEFFECT_AE_DELAY_PROCESSOR_UTILITIES_H

#pragma once
#include "libaa/audio_generator/aa_lfo_generator.h"
#include "libaa/core/aa_audio_buffer.h"
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_delay_lines.h"
namespace libaa {
class DelayProcessorUtilities {
public:
    void prepare(int max_delay_samples);

    size_t getNumDelayLines() const;

    size_t getMaxDelaySamples() const;

    void process(AudioBufferX<float> *buffer, int delay_samples, float feedback,
                 float dry, float wet);

    using DelayGenerator = std::function<float()>;
    void processWithDelayGenerator(AudioBufferX<float> *buffer,
                                   DelayGenerator &&g, float feedback,
                                   float dry, float wet);

private:
    DelayLines<float> dlines_;
};
} // namespace libaa

#endif // AUDIOEFFECT_AE_DELAY_PROCESSOR_UTILITIES_H
