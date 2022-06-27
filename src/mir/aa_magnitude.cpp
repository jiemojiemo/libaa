//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_magnitude.h"
#include <Eigen/Dense>

namespace libaa::MIR {
Magnitude::Magnitude() {
    defineInput(in_complex_, "complex");
    defineOutput(out_mag_, "magnitude");
}

std::string Magnitude::getName() const {
    return {"Magnitude"};
}
void Magnitude::configure() {
}
void Magnitude::compute() {
    const auto &in_complex = in_complex_.get();
    auto &out_mag = out_mag_.get();

    out_mag.resize(in_complex.size());

    Eigen::Map<const Eigen::ArrayXcf> complex_array(in_complex.data(), in_complex.size());
    Eigen::Map<Eigen::ArrayXf> mag_array(out_mag.data(), out_mag.size());

    mag_array = complex_array.abs();
}
} // namespace libaa::MIR