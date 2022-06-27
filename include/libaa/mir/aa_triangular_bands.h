//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_TRIANGULAR_BANDS_H
#define LIBAA_AA_TRIANGULAR_BANDS_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class TriangularBands : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_spec_;
    Output<std::vector<float>> out_bands_;

public:
    TriangularBands();
    ~TriangularBands() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const std::vector<float> &getFrequencies() const;
    int getNumberBands() const;
    const void *getFilters() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl_{nullptr};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_TRIANGULAR_BANDS_H
