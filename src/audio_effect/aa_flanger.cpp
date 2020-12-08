//
// Created by william on 2020/3/3.
//

#include "audio_effect/aa_flanger.h"
#include "dsp/aa_lfo.h"
#include "audio_basics/aa_delay_line_array.h"
namespace libaa
{

class Flanger::Impl
{
public:
    float phase_={0.0f};
    float invert_sample_rate_{0.0f};
    LFO lfo_;
    DelayLineArray<float> dlines_;
};

Flanger::Flanger()
    : impl_(std::make_shared<Impl>())
{

}

void Flanger::prepareToPlay(double sample_rate, int max_block_size)
{
    impl_->invert_sample_rate_ = 1.0/sample_rate;

    const int num_supported_channel = 2;
    const float max_delay_length = 2.0f; // 2 seconds
    const auto max_delay_length_sample = static_cast<size_t>(max_delay_length * sample_rate);

    impl_->dlines_.allocateDelayLines(num_supported_channel, max_delay_length_sample);
}
void Flanger::reset()
{
    impl_->dlines_.clear();
}
void Flanger::releaseResources()
{

}
void Flanger::processBlock(AudioBuffer<float> &buffer)
{
    const int num_channels = buffer.getNumChannels();
    const int num_samples = buffer.getNumFrames();
    float phase = 0.0f;
    float channel0EndPhase = impl_->phase_;

    assert(static_cast<size_t>(num_channels) <= impl_->dlines_.getNumLines());

    for(int c = 0; c < num_channels; ++c)
    {
        phase = impl_->phase_;
        if(stereo != 0 && c != 0)
        {
            phase = fmodf(phase + 0.25f, 1.0f);
        }

        float* channel_data = buffer.getWritePointer(c);
        auto* dline = impl_->dlines_.getDelayLine(c);

        for(int i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];

            // get delay from lfo
            float delay_second = min_delay + sweep_width*impl_->lfo_.lfo(phase, LFO::WaveformType::kWaveformSine);
            float delay_sample = delay_second * static_cast<float>(getSampleRate());

            // get interpolation delay value
            float interpolation_val = dline->getInterpolation(delay_sample);
            channel_data[i] = in + depth * interpolation_val;

            // push input to delay line
            dline->push(in + (interpolation_val * feedback));

            // update phase
            phase += lfo_freq*impl_->invert_sample_rate_;
            if(phase >= 1.0f)
            {
                phase -= 1.0f;
            }
        }

        // use channel 0 only keep the phase in sync between call processBlock()
        if(c == 0)
        {
            channel0EndPhase = phase;
        }
    }

    impl_->phase_ = channel0EndPhase;
}

}
