//
// Created by user on 4/16/22.
//

#include "libaa/dsp/aa_simple_delay.h"

namespace libaa {
SimpleDelay::SimpleDelay(size_t delay_line_size) {
    dline_.resize(delay_line_size);
}
void SimpleDelay::prepare(float sample_rate) {
    sample_rate_ = sample_rate;
}

void SimpleDelay::resize(size_t new_size) {
    dline_.resize(new_size);
}

size_t SimpleDelay::getSize() const {
    return dline_.size();
}

void SimpleDelay::updateParameters(SimpleDelayParameters param) {
    parameters = param;
    num_delay_samples_ = parameters.delay_ms / 1000.0f * sample_rate_;
}

const SimpleDelay::SimpleDelayParameters &SimpleDelay::getParameters() const {
    return parameters;
}

float SimpleDelay::processSample(float in) {
    dline_.push(in);

    float d_y = parameters.interpolate
                    ? dline_.getInterpolation(num_delay_samples_)
                    : dline_.get(int(num_delay_samples_));

    return d_y;
}
} // namespace libaa