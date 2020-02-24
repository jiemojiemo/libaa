//
// Created by william on 2020/2/21.
//

#include "audio_effect/aa_delay_effect.h"
namespace libaa
{

void DelayEffect::prepareToPlay(double sample_rate, int samplers_per_block)
{
    int num_new_channel = 2;

    dlines_.resize(num_new_channel);
    delay_length_in_sample_.resize(num_new_channel, static_cast<size_t>(delay_length_ * sample_rate));

    for(int i = 0; i < num_new_channel; ++i)
    {
        dlines_[i].resize(static_cast<size_t>(2.0 * sample_rate));
    }

}

void DelayEffect::reset()
{
    for(auto& d : dlines_){ d.clear(); }
}

void DelayEffect::releaseResources()
{
    for(auto& d : dlines_){ d.clear(); }
}

void DelayEffect::processBlock(AudioBuffer<float> &buffer)
{
    const int num_channels = buffer.getNumChannels();
    const int num_samples = buffer.getNumSamples();

    for(int c = 0; c < num_channels; ++c)
    {
        float* channel_data = buffer.getWritePointer(c);
        auto& dline = dlines_[c];
        size_t delay_samples = delay_length_in_sample_[c];

        for(int i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];
            const float delay_val = dline.get(delay_samples);
            float out = 0.0f;

            out = (dry_mix_ * in + wet_mix_ * delay_val);

            dline.push( in + feedback_ * delay_val);

            channel_data[i] = out;
        }
    }
}
}
