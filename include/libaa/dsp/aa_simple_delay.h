//
// Created by user on 4/16/22.
//

#ifndef LIBAA_AA_SIMPLE_DELAY_H
#define LIBAA_AA_SIMPLE_DELAY_H

#pragma once
#include "libaa/core/aa_delay_line.h"

namespace libaa {

class SimpleDelay {
public:
    explicit SimpleDelay(size_t delay_line_size = 0);

    void prepare(float sample_rate);

    void resize(size_t new_size);

    size_t getSize() const;

    struct SimpleDelayParameters {
        float delay_ms;
        bool interpolate;
    };
    void updateParameters(SimpleDelayParameters param);

    const SimpleDelayParameters &getParameters() const;

    float processSample(float in);
private:
    SimpleDelayParameters parameters;

    DelayLine<float> dline_;
    float sample_rate_{0.0f};
    float num_delay_samples_{0};
};
} // namespace libaa

#endif // LIBAA_AA_SIMPLE_DELAY_H