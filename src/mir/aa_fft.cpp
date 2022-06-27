//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_fft.h"
#include "dsp/FFT.h"
#include <stdexcept>
namespace libaa {
namespace MIR {
FFT::FFT() {
    parameters_.pushIntParameter("size", 1024, 1, std::numeric_limits<int>::max());

    defineInput(in_frame_, "frame");
    defineOutput(out_fft_, "fft");
}

std::string FFT::getName() const {
    return "FFT";
}

void FFT::configure() {
    auto size = getParameter("size").getInt();
    fft_ = std::make_shared<RubberBand::FFT>(size);
    fft_->initFloat();
}

void FFT::compute() {
    const std::vector<float> &in_frame = in_frame_.get();
    std::vector<std::complex<float>> &out_fft = out_fft_.get();

    size_t size = getParameter("size").getInt();

    if (in_frame.size() < size) {
        throw std::invalid_argument("invalid input signal, size <= fft_size");
    }

    const int half_size = size / 2 + 1;

    out_fft.resize(half_size);
    fft_->forwardComplex(in_frame.data(), out_fft.data());
}

// -- testing ---
void *FFT::getFFTImpl() const {
    return fft_.get();
}
} // namespace MIR
} // namespace AudioEffect
