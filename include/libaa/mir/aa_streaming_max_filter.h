//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_STREAMING_MAX_FILTER_H
#define LIBAA_AA_STREAMING_MAX_FILTER_H

#pragma once
#include "libaa/mir/aa_max_filter.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
namespace libaa::MIR {
class StreamingMaxFilter : public MIRAlgorithmBase {
protected:
    Input<float> in_signal_;
    Output<float> out_signal_;

public:
    StreamingMaxFilter();
    ~StreamingMaxFilter() override = default;
    void reset() override;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    const MaxFilter::SimpleRingBuffer *getSimpleRingBuffer() const;

private:
    std::unique_ptr<MaxFilter::SimpleRingBuffer> buffer_{nullptr};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_STREAMING_MAX_FILTER_H
