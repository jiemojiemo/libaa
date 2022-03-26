//
// Created by William on 3/26/22.
//

#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_delay_lines.h"
#include "libaa/processor/ae_delay_processor_utilities.h"
#include <cassert>
namespace libaa {
void DelayProcessorUtilities::prepare(int max_delay_samples) {
    const int kMaxNumberChannel = 2;
    dlines_.allocateDelayLines(kMaxNumberChannel, max_delay_samples);
}

size_t DelayProcessorUtilities::getNumDelayLines() const {
    return dlines_.getNumDelayLines();
}

size_t DelayProcessorUtilities::getMaxDelaySamples() const {
    return dlines_.getMaxDelaySamples();
}

void DelayProcessorUtilities::process(AudioBufferX<float> *buffer,
                                      int delay_samples, float feedback,
                                      float dry, float wet) {

    for (size_t c = 0; c < buffer->getNumberChannels(); ++c) {
        float *channel = buffer->getWriterPointer(c);
        auto *dline = dlines_.getDelayLine(c);
        assert(dline != nullptr);
        for (size_t i = 0; i < buffer->getNumberFrames(); ++i) {
            float in = channel[i];
            float d_y = dline->get(delay_samples);
            float d_x = in + feedback * d_y;

            dline->push(d_x);

            channel[i] = dry * in + wet * d_y;
        }
    }
}

void DelayProcessorUtilities::processWithDelayGenerator(
    AudioBufferX<float> *buffer, DelayGenerator &&g, float feedback, float dry,
    float wet) {
    for (size_t i = 0; i < buffer->getNumberFrames(); ++i) {
        float delay_num_samples = g();
        for (size_t c = 0; c < buffer->getNumberChannels(); ++c) {
            float *channel = buffer->getWriterPointer(c);
            auto *dline = dlines_.getDelayLine(c);

            float in = channel[i];
            float d_y = dline->getInterpolation(delay_num_samples);
            float d_x = in + feedback * d_y;

            dline->push(d_x);

            float out = dry * in + wet * d_y;
            channel[i] = out;
        }
    }
}

} // namespace libaa
