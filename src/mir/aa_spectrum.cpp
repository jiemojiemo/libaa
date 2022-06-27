//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_spectrum.h"

namespace libaa::MIR {
Spectrum::Spectrum() {
    parameters_.pushIntParameter("size", 2048, 1, std::numeric_limits<int>::max());

    defineInput(in_frame_, "frame");
    defineOutput(out_spectrum_, "spectrum");

    fft_ = MIRAlgorithmFactory::create("FFT");
    magnitude_ = MIRAlgorithmFactory::create("Magnitude");
}
std::string Spectrum::getName() const {
    return "Spectrum";
}
void Spectrum::configure() {
    int size = MIRAlgorithmBase::getParameter("size").getInt();
    const int half_size = size / 2 + 1;
    fft_buffer_.resize(half_size);

    fft_->setParameter("size", size);
    fft_->configure();
    fft_->output("fft").set(fft_buffer_);

    magnitude_->input("complex").set(fft_buffer_);
}
void Spectrum::compute() {
    const auto &in_frame = in_frame_.get();
    auto &out_spec = out_spectrum_.get();

    fft_->input("frame").set(in_frame);
    magnitude_->output("magnitude").set(out_spec);

    fft_->compute();
    magnitude_->compute();
}

const MIRAlgorithmBase *Spectrum::getFFT() const {
    return dynamic_cast<MIRAlgorithmBase *>(fft_.get());
}

const MIRAlgorithmBase *Spectrum::getMagnitude() const {
    return dynamic_cast<MIRAlgorithmBase *>(magnitude_.get());
}
} // namespace AudioEffect::MIR
