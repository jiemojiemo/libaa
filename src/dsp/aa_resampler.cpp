//
// Created by user on 5/16/22.
//

#include "libaa/dsp/aa_resampler.h"
#include "dsp/Resampler.h"
#include <cassert>
namespace libaa {
class Resampler::Impl {
public:
    std::unique_ptr<RubberBand::Resampler> resampler_{nullptr};
    int num_channels_{0};
};

Resampler::Resampler()
    : impl_(std::make_shared<Impl>()) {}

void Resampler::prepare(float sample_rate, int num_channels,
                        int max_block_size) {
    impl_->num_channels_ = num_channels;

    RubberBand::Resampler::Parameters param;
    param.initialSampleRate = sample_rate;
    param.debugLevel = 0;
    param.maxBufferSize = max_block_size;

    impl_->resampler_ =
        std::make_unique<RubberBand::Resampler>(param, num_channels);
}

int Resampler::resample(float **in, int in_num_frames, float **out,
                        int out_num_frames, float ratio, bool is_final) {
    assert(impl_->resampler_ != nullptr);
    return impl_->resampler_->resample(out, out_num_frames, in, in_num_frames,
                                       ratio, is_final);
}

int Resampler::resampleInterleaved(float *in, int in_num_frames, float *out,
                                   int out_num_frames, float ratio,
                                   bool is_final) {
    assert(impl_->resampler_ != nullptr);
    return impl_->resampler_->resampleInterleaved(
        out, out_num_frames, in, in_num_frames, ratio, is_final);
}

int Resampler::getNumberChannels() const {
    return impl_->num_channels_;
}

void *Resampler::getResampleImpl() const {
    return impl_->resampler_.get();
}

} // namespace libaa
