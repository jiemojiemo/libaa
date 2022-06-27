//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_SPECTRUM_H
#define LIBAA_AA_SPECTRUM_H

#pragma once
#include "libaa/mir/aa_fft.h"
#include "libaa/mir/aa_magnitude.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
namespace libaa::MIR {
class Spectrum : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_frame_;
    Output<std::vector<float>> out_spectrum_;

public:
    Spectrum();
    ~Spectrum() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const MIRAlgorithmBase *getFFT() const;
    const MIRAlgorithmBase *getMagnitude() const;

private:
    std::vector<std::complex<float>> fft_buffer_;
    std::unique_ptr<IMIRAlgorithm> fft_{nullptr};
    std::unique_ptr<IMIRAlgorithm> magnitude_{nullptr};
};
} // namespace libaa::MIR

#endif // LIBAA_AA_SPECTRUM_H
