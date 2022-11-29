//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_STREAMING_SUPERFLUX_NOVELTY_H
#define LIBAA_AA_STREAMING_SUPERFLUX_NOVELTY_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class StreamingSuperFluxNovelty : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_bands_;
    Output<float> out_differences_;

public:
    StreamingSuperFluxNovelty();
    ~StreamingSuperFluxNovelty() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;
    void reset() override;

    // -- testing --
    const MIRAlgorithmBase *getSuperFluxNovelty() const;
    const std::vector<std::vector<float>> &getRingBands() const;

private:
    size_t frame_width_{0};
    std::unique_ptr<IMIRAlgorithm> super_flux_novelty_{nullptr};
    std::vector<std::vector<float>> ring_bands_;
    float super_flux_out_{0.0f};
};
} // namespace libaa::MIR

#endif // LIBAA_AA_STREAMING_SUPERFLUX_NOVELTY_H
