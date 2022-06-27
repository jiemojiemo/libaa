//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_SUPERFLUX_PEAKS_H
#define LIBAA_AA_SUPERFLUX_PEAKS_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class SuperFluxPeaks : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_novelty_;
    Output<std::vector<float>> out_peaks_;

public:
    SuperFluxPeaks();
    ~SuperFluxPeaks() override = default;

    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const MIRAlgorithmBase *getMovingAverage() const;

    const MIRAlgorithmBase *getMaxFilter() const;

private:
    static bool isLocalMax(float in, float in_max);

    static bool isOverThreshold(float in, float in_ave, float threshold, float ratio_threshold);

    std::unique_ptr<IMIRAlgorithm> moving_ave_{nullptr};
    std::unique_ptr<IMIRAlgorithm> max_filter_{nullptr};
    float start_peak_time_{0.0f};
    float combine_{0};
    float frame_rate_{0};
    float ratio_threshold_{0};
    float threshold_{0};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_SUPERFLUX_PEAKS_H
