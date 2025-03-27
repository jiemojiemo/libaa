//
// Created by user on 3/27/25.
//


#include "libaa/offline/aa_time_stretching.h"
#include "RubberBandStretcher.h"
#include "libaa/core/aa_math_tools.h"
#include <cfloat>

namespace libaa {
class TimeStretching::Impl {
public:
    std::unique_ptr<RubberBand::RubberBandStretcher> rubberband_{nullptr};
    AudioProcessorParameters parameters_;
};

TimeStretching::TimeStretching()
    : impl_(std::make_shared<Impl>()) {
    impl_->parameters_.pushFloatParameter("Time Ratio", 1.0f, 0.1f, 10.0f);
    impl_->parameters_.pushFloatParameter("Pitch Scale", 1.0f, 0.1f, 8.0f);
}

std::string TimeStretching::getName() const {
    return {"TimeStretching"};
}

void TimeStretching::prepare(int sample_rate, int num_channels,
                             int max_block_size) {
    using namespace RubberBand;
    RubberBandStretcher::Options options = 0;
    options |= RubberBandStretcher::OptionProcessRealTime;
    options |= RubberBandStretcher::OptionStretchPrecise;
    options |= RubberBandStretcher::OptionTransientsSmooth;
    options |= RubberBandStretcher::OptionDetectorCompound;

    impl_->rubberband_ = std::make_unique<RubberBandStretcher>(
        sample_rate, num_channels, options);
    impl_->rubberband_->setMaxProcessSize(max_block_size);
}

int TimeStretching::available() const {
    if (impl_->rubberband_ != nullptr) {
        return impl_->rubberband_->available();
    }
    return 0;
}

void TimeStretching::process(const float *const *input, size_t samples,
                             bool final) {
    if (impl_->rubberband_ == nullptr)
        return;

    updateInternalParametersBeforeProcess();

    impl_->rubberband_->process(input, samples, final);
}

size_t TimeStretching::retrieve(float *const *output, size_t samples) const {
    if (impl_->rubberband_ == nullptr)
        return 0;

    return impl_->rubberband_->retrieve(output, samples);
}

const AudioProcessorParameters *TimeStretching::getParameters() const {
    return &impl_->parameters_;
}

AudioProcessorParameters *TimeStretching::getParameters() {
    return &impl_->parameters_;
}

void *TimeStretching::getTsImpl() const {
    return impl_->rubberband_.get();
}

bool TimeStretching::pushParameterChangePoint(const ParameterChangePoint &p) {
    if (p.index >= static_cast<int>(impl_->parameters_.size())) {
        return false;
    }
    impl_->parameters_.get(p.index).setNormalizedValue(p.normalized_value);
    return true;
}

void TimeStretching::updateInternalParametersBeforeProcess() {
    // update time ratio or pitch scale
    auto new_time_ratio = impl_->parameters_.get(0).getPlainValue();
    if (!approximatelyEqual(impl_->rubberband_->getTimeRatio(), new_time_ratio,
                            FLT_EPSILON)) {
        impl_->rubberband_->setTimeRatio(new_time_ratio);
    }

    auto new_pitch_scale = impl_->parameters_.get(1).getPlainValue();
    if (!approximatelyEqual(impl_->rubberband_->getPitchScale(),
                            new_pitch_scale, FLT_EPSILON)) {
        impl_->rubberband_->setPitchScale(new_pitch_scale);
    }
}

} // namespace AudioEffect