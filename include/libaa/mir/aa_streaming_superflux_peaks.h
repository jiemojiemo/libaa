//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_STREAMING_SUPERFLUX_PEAKS_H
#define LIBAA_AA_STREAMING_SUPERFLUX_PEAKS_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class StreamingSuperFluxPeaks : public MIRAlgorithmBase {
protected:
    Input<float> in_novelty_;
    Output<std::vector<float>> out_peaks_;

public:
    StreamingSuperFluxPeaks();
    ~StreamingSuperFluxPeaks() override = default;
    void reset() override;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const MIRAlgorithmBase *getStreamingMovingAverage() const;

    const MIRAlgorithmBase *getStreamingMaxFilter() const;

    float getStartPeakTime() const;

private:
    bool isLocalMax(float in, float in_max);

    bool isOverThreshold(float in, float in_ave, float threshold, float ratio_threshold);

    std::unique_ptr<IMIRAlgorithm> moving_ave_{nullptr};
    std::unique_ptr<IMIRAlgorithm> max_filter_{nullptr};
    float in_{0.0f};
    float local_average_{0.0f};
    float local_max_{0.0f};
    float last_peak_time_{-99999}; // very small very for the first peak

    float start_peak_time_{0.0f};
    float combine_{0};
    float frame_rate_{0};
    float ratio_threshold_{0};
    float threshold_{0};
};
} // namespace libaa::MIR

#endif // LIBAA_AA_STREAMING_SUPERFLUX_PEAKS_H
