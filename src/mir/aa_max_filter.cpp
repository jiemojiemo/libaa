//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_max_filter.h"
#include <algorithm>

namespace libaa::MIR {

MaxFilter::MaxFilter() {
    parameters_.pushIntParameter("width", 3, 2, std::numeric_limits<int>::max());
    defineInput(in_signal_, "signal");
    defineOutput(out_signal_, "signal");
}
std::string MaxFilter::getName() const {
    return "MaxFilter";
}
void MaxFilter::configure() {
    auto width = MIRAlgorithmBase::getParameter("width").getInt();

    buffer_ = std::make_unique<SimpleRingBuffer>(width, std::numeric_limits<float>::lowest());
}
void MaxFilter::compute() {
    const auto &in_signal = in_signal_.get();
    auto &out_signal = out_signal_.get();

    out_signal.resize(in_signal.size());

    for (auto i = 0u; i < in_signal.size(); ++i) {
        buffer_->push(in_signal[i]);
        out_signal[i] = *std::max_element(buffer_->data.begin(), buffer_->data.end());
    }
}

// -- testing --
const MaxFilter::SimpleRingBuffer *MaxFilter::getSimpleRingBuffer() const {
    return buffer_.get();
}
} // namespace libaa::MIR