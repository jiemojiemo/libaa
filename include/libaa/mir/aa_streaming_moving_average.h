//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_STREAMING_MOVING_AVERAGE_H
#define LIBAA_AA_STREAMING_MOVING_AVERAGE_H

#pragma once
#include "libaa/core/aa_delay_line.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
namespace libaa::MIR {
class StreamingMovingAverage : public MIRAlgorithmBase {
protected:
    Input<float> in_signal_;
    Output<float> out_signal_;

public:
    StreamingMovingAverage();
    ~StreamingMovingAverage() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;
    void reset() override;

    // -- testing --
    float getPreMean() const;

    const DelayLine<float> &getDelayLine() const;

private:
    int size_;
    float pre_mean_{0.0f};
    DelayLine<float> dline_;
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_STREAMING_MOVING_AVERAGE_H
