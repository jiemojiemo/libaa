//
// Created by william on 2020/2/21.
//

#include "audio_effect/aa_delay_effect.h"
#include "audio_basics/aa_delay_line_array.h"
namespace libaa
{
class DelayEffect::Impl
{
public:
//    std::vector<DelayLine<float>> dlines_;
    std::vector<size_t> delay_length_in_sample_;
    DelayLineArray<float> dlines_;
};


void DelayEffect::prepareToPlay(double sample_rate, int max_block_size)
{
    int num_new_channel = 2;

    const auto max_delay_length = static_cast<size_t>(2.0 * sample_rate);

    impl_->dlines_.allocateDelayLines(num_new_channel, max_delay_length);
    impl_->delay_length_in_sample_.resize(num_new_channel, static_cast<size_t>(delay_length_ * sample_rate));

}

void DelayEffect::reset()
{
    impl_->dlines_.clear();
}

void DelayEffect::releaseResources()
{
    impl_->dlines_.clear();
}

void DelayEffect::processBlock(AudioBuffer<float> &buffer)
{
    const int num_channels = buffer.getNumChannels();
    const int num_samples = buffer.getNumFrames();

    for(int c = 0; c < num_channels; ++c)
    {
        float* channel_data = buffer.getWritePointer(c);
        auto* dline = impl_->dlines_.getDelayLine(c);
        size_t delay_samples = impl_->delay_length_in_sample_[c];

        for(int i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];
            const float delay_val = dline->get(delay_samples);
            float out = 0.0f;

            out = (dry_mix_ * in + wet_mix_ * delay_val);

            dline->push( in + feedback_ * delay_val);

            channel_data[i] = out;
        }
    }
}
DelayEffect::DelayEffect()
    : impl_(std::make_shared<Impl>())
{

}
}
