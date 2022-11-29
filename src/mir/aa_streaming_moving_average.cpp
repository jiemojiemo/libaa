//
// Created by user on 11/29/22.
//
#include "libaa/mir/aa_streaming_moving_average.h"
namespace libaa::MIR {
StreamingMovingAverage::StreamingMovingAverage() {
    parameters_.pushIntParameter("size", 6, 1, std::numeric_limits<int>::max());

    defineInput(in_signal_, "signal");
    defineOutput(out_signal_, "signal");
}
std::string StreamingMovingAverage::getName() const {
    return "StreamingMovingAverage";
}
void StreamingMovingAverage::configure() {
    size_ = getParameter("size").getInt();
    dline_.resize(size_);
}
void StreamingMovingAverage::compute() {
    const auto &in = in_signal_.get();
    auto &out = out_signal_.get();

    dline_.push(in);
    float pre_value = dline_.get(size_);

    out = (pre_mean_ * size_ + in - pre_value) / size_;
    pre_mean_ = out;
}
void StreamingMovingAverage::reset() {
    pre_mean_ = 0.0f;
}

// -- testing --
float StreamingMovingAverage::getPreMean() const {
    return pre_mean_;
}

const DelayLine<float> &StreamingMovingAverage::getDelayLine() const {
    return dline_;
}

} // namespace AudioEffect::MIR
