//
// Created by user on 11/29/22.
//
#include "libaa/mir/aa_streaming_max_filter.h"

namespace libaa::MIR {
StreamingMaxFilter::StreamingMaxFilter() {
    parameters_.pushIntParameter("width", 3, 2, std::numeric_limits<int>::max());

    defineInput(in_signal_, "signal");
    defineOutput(out_signal_, "signal");
}

void StreamingMaxFilter::reset() {
    IMIRAlgorithm::reset();
}

std::string StreamingMaxFilter::getName() const {
    return "StreamingMaxFilter";
}

void StreamingMaxFilter::configure() {
    auto width = MIRAlgorithmBase::getParameter("width").getInt();
    buffer_ = std::make_unique<MaxFilter::SimpleRingBuffer>(width, std::numeric_limits<float>::lowest());
}

void StreamingMaxFilter::compute() {
    const auto &in = in_signal_.get();
    auto &out = out_signal_.get();

    buffer_->push(in);
    out = *std::max_element(buffer_->data.begin(), buffer_->data.end());
}

const MaxFilter::SimpleRingBuffer *StreamingMaxFilter::getSimpleRingBuffer() const {
    return buffer_.get();
}
} // namespace AudioEffect::MIR