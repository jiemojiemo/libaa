//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_superflux_peaks.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include <limits>
namespace libaa::MIR {
SuperFluxPeaks::SuperFluxPeaks() {
    parameters_.pushFloatParameter("combine", 30, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("frameRate", 172.265, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("preAverage", 100, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("preMax", 30, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("ratioThreshold", 16, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("threshold", 0.05, 0, std::numeric_limits<float>::max());
    defineInput(in_novelty_, "novelty");
    defineOutput(out_peaks_, "peaks");

    moving_ave_ = MIRAlgorithmFactory::create("MovingAverage");
    max_filter_ = MIRAlgorithmFactory::create("MaxFilter");
}

std::string SuperFluxPeaks::getName() const {
    return {"SuperFluxPeaks"};
}
void SuperFluxPeaks::configure() {
    combine_ = getParameter("combine").getPlainValue() / 1000.0f;
    frame_rate_ = getParameter("frameRate").getPlainValue();
    float pre_ave_ms = getParameter("preAverage").getPlainValue();
    float pre_max_ms = getParameter("preMax").getPlainValue();
    ratio_threshold_ = getParameter("ratioThreshold").getPlainValue();
    threshold_ = getParameter("threshold").getPlainValue();
    start_peak_time_ = 0.0f;

    int pre_ave_size = static_cast<int>(frame_rate_ * pre_ave_ms / 1000.0f);
    moving_ave_->setParameter("size", pre_ave_size);

    int pre_max_size = static_cast<int>(frame_rate_ * pre_max_ms / 1000.0f);
    max_filter_->setParameter("width", pre_max_size);

    moving_ave_->configure();
    max_filter_->configure();
}
void SuperFluxPeaks::compute() {
    const auto &in_novelty = in_novelty_.get();
    auto &peaks = out_peaks_.get();

    size_t in_size = in_novelty.size();

    std::vector<float> avg(in_size);
    moving_ave_->input("signal").set(in_novelty);
    moving_ave_->output("signal").set(avg);
    moving_ave_->compute();

    std::vector<float> maxs(in_size);
    max_filter_->input("signal").set(in_novelty);
    max_filter_->output("signal").set(maxs);
    max_filter_->compute();

    peaks.resize(0);
    for (auto i = 0u; i < in_size; ++i) {
        if (isLocalMax(in_novelty[i], maxs[i])) {

            if (isOverThreshold(in_novelty[i], avg[i], threshold_, ratio_threshold_)) {
                float peak_time = start_peak_time_ + i / frame_rate_;
                if (peaks.empty()) {
                    peaks.push_back(peak_time);
                } else {
                    float last_peak = peaks.back();
                    if (peak_time - last_peak > combine_) {
                        peaks.push_back(peak_time);
                    }
                }
            }
        }
    }
}

// -- testing --
const MIRAlgorithmBase *SuperFluxPeaks::getMovingAverage() const {
    return dynamic_cast<MIRAlgorithmBase *>(moving_ave_.get());
}

const MIRAlgorithmBase *SuperFluxPeaks::getMaxFilter() const {
    return dynamic_cast<MIRAlgorithmBase *>(max_filter_.get());
}

bool SuperFluxPeaks::isLocalMax(float in, float in_max) {
    static const float kNoiseThreshold = 1e-8;
    return in == in_max && in > kNoiseThreshold;
}

bool SuperFluxPeaks::isOverThreshold(float in, float in_ave, float threshold, float ratio_threshold) {
    bool is_over_linear_threshold = (in > (in_ave + threshold));
    bool is_over_ratio_threshold = (in_ave > 0 && in / in_ave > ratio_threshold);
    return is_over_linear_threshold || is_over_ratio_threshold;
}

} // namespace AudioEffect::MIR

