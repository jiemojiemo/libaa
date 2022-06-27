//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_FFT_H
#define LIBAA_AA_FFT_H
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <complex>
#include <memory>
namespace RubberBand {
class FFT;
}
namespace libaa {
namespace MIR {
class FFT : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_frame_;
    Output<std::vector<std::complex<float>>> out_fft_;

public:
    FFT();

    ~FFT() override = default;

    std::string getName() const override;

    void configure() override;

    void compute() override;

    // -- testing ---
    void *getFFTImpl() const;

private:
    std::shared_ptr<RubberBand::FFT> fft_{nullptr};
};
} // namespace MIR
} // namespace AudioEffect

#endif // LIBAA_AA_FFT_H
