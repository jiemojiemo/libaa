//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_SUPERFLUX_NOVELTY_H
#define LIBAA_AA_SUPERFLUX_NOVELTY_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class SuperFluxNovelty : public MIRAlgorithmBase {
protected:
    Input<std::vector<std::vector<float>>> in_bands_;
    Output<float> out_differences_;

public:
    SuperFluxNovelty();
    ~SuperFluxNovelty() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const MIRAlgorithmBase *getMaxFilter() const;

private:
    std::unique_ptr<IMIRAlgorithm> max_filter_{nullptr};
    int frame_width_{0};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_SUPERFLUX_NOVELTY_H
