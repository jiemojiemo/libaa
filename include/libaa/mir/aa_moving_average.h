//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MOVING_AVERAGE_H
#define LIBAA_AA_MOVING_AVERAGE_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
namespace libaa::MIR {
class MovingAverage : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_signal_;
    Output<std::vector<float>> out_signal_;

public:
    MovingAverage();
    ~MovingAverage() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;
    void reset() override;

    // -- testing --
    float getPreMean() const;

private:
    int size_;
    float pre_mean_{0.0f};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_MOVING_AVERAGE_H
