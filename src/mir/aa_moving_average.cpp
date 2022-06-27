//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_moving_average.h"

#include <limits>
namespace libaa::MIR {
MovingAverage::MovingAverage() {
    parameters_.pushIntParameter("size", 6, 1, std::numeric_limits<int>::max());

    defineInput(in_signal_, "signal");
    defineOutput(out_signal_, "signal");
}

std::string MovingAverage::getName() const {
    return {"MovingAverage"};
}
void MovingAverage::configure() {
    size_ = getParameter("size").getInt();
}
void MovingAverage::compute() {
    const auto &in_signal = in_signal_.get();
    auto &out_signal = out_signal_.get();

    out_signal.resize(in_signal.size());

    for (int i = 0; i < static_cast<int>(in_signal.size()); ++i) {
        int pre_local_index = i - size_;
        float pre_value = (pre_local_index < 0) ? 0 : in_signal[pre_local_index];

        out_signal[i] = (pre_mean_ * size_ + in_signal[i] - pre_value) / size_;
        pre_mean_ = out_signal[i];
    }
}
void MovingAverage::reset() {
    pre_mean_ = .0f;
}
// -- testing --
float MovingAverage::getPreMean() const {
    return pre_mean_;
}

}