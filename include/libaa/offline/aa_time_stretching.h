//
// Created by user on 3/27/25.
//

#ifndef AA_TIME_STRETCHING_H
#define AA_TIME_STRETCHING_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_parameter_changes.h"

namespace libaa {
class TimeStretching {
public:
    TimeStretching();

    std::string getName() const;

    void prepare(int sample_rate, int num_channels, int max_block_size);

    int available() const;

    void process(const float *const *input, size_t samples, bool final);

    size_t retrieve(float *const *output, size_t samples) const;

    const AudioProcessorParameters *getParameters() const;
    AudioProcessorParameters *getParameters();

    void *getTsImpl() const;

    bool pushParameterChangePoint(const ParameterChangePoint &p);

private:
    void updateInternalParametersBeforeProcess();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace AudioEffect

#endif //AA_TIME_STRETCHING_H
