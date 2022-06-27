//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MAGNITUDE_H
#define LIBAA_AA_MAGNITUDE_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <complex>
namespace libaa::MIR {
class Magnitude : public MIRAlgorithmBase {
protected:
    Input<std::vector<std::complex<float>>> in_complex_;
    Output<std::vector<float>> out_mag_;

public:
    Magnitude();
    ~Magnitude() override = default;

    std::string getName() const override;
    void configure() override;
    void compute() override;
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_MAGNITUDE_H
