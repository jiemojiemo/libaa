//
// Created by user on 11/29/22.
//

#include "libaa/mir/aa_streaming_superflux_peaks.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include <limits>
namespace libaa::MIR {
StreamingSuperFluxPeaks::StreamingSuperFluxPeaks() {
    parameters_.pushFloatParameter("combine", 30, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("frameRate", 172.265, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("preAverage", 100, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("preMax", 30, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("ratioThreshold", 16, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("threshold", 0.05, 0, std::numeric_limits<float>::max());

    defineInput(in_novelty_, "novelty");
    defineOutput(out_peaks_, "peaks");

    moving_ave_ = MIRAlgorithmFactory::create("StreamingMovingAverage");
    max_filter_ = MIRAlgorithmFactory::create("StreamingMaxFilter");
}

void StreamingSuperFluxPeaks::reset() {
    max_filter_->reset();
    moving_ave_->reset();

    start_peak_time_ = 0.0f;
}

std::string StreamingSuperFluxPeaks::getName() const {
    return "StreamingSuperFluxPeaks";
}

void StreamingSuperFluxPeaks::configure() {
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

    moving_ave_->input("signal").set(in_);
    moving_ave_->output("signal").set(local_average_);

    max_filter_->input("signal").set(in_);
    max_filter_->output("signal").set(local_max_);

    moving_ave_->configure();
    max_filter_->configure();
}

void StreamingSuperFluxPeaks::compute() {
    const auto &in_novelty = in_novelty_.get();
    auto &peaks = out_peaks_.get();

    in_ = in_novelty;

    moving_ave_->compute();
    max_filter_->compute();

    peaks.resize(0);
    if (isLocalMax(in_, local_max_)) {
        if (isOverThreshold(in_, local_average_, threshold_, ratio_threshold_)) {
            float peak_time = start_peak_time_;
            if (peak_time - last_peak_time_ > combine_) {
                peaks.push_back(peak_time);
                last_peak_time_ = peak_time;
            }
        }
    }

    start_peak_time_ += 1.0f / frame_rate_;
}

// -- testing --
const MIRAlgorithmBase *StreamingSuperFluxPeaks::getStreamingMovingAverage() const {
    return dynamic_cast<MIRAlgorithmBase *>(moving_ave_.get());
}

const MIRAlgorithmBase *StreamingSuperFluxPeaks::getStreamingMaxFilter() const {
    return dynamic_cast<MIRAlgorithmBase *>(max_filter_.get());
}

float StreamingSuperFluxPeaks::getStartPeakTime() const {
    return start_peak_time_;
}

bool StreamingSuperFluxPeaks::isLocalMax(float in, float in_max) {
    static const float kNoiseThreshold = 1e-8;
    return in == in_max && in > kNoiseThreshold;
}

bool StreamingSuperFluxPeaks::isOverThreshold(float in, float in_ave, float threshold, float ratio_threshold) {
    bool is_over_linear_threshold = (in > (in_ave + threshold));
    bool is_over_ratio_threshold = (in_ave > 0 && in / in_ave > ratio_threshold);
    return is_over_linear_threshold || is_over_ratio_threshold;
}

} // namespace libaa::MIR
