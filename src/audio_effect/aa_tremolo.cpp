//
// Created by william on 2020/6/4.
//

#include "audio_effect/aa_tremolo.h"

namespace libaa
{
class Tremolo::Impl
{
public:
    float lfo_phase_;
    float invert_sample_rate_;
    LFO lfo_;
};

Tremolo::Tremolo():
    impl_(std::make_shared<Impl>())
{
}

std::string Tremolo::getName() const
{
    return std::string("tremolo");
}
void Tremolo::prepareToPlay(double sample_rate, int max_block_size)
{
    impl_->lfo_phase_ = 0;
    impl_->invert_sample_rate_ = 1.0 / sample_rate;
}
void Tremolo::reset()  {
    impl_->lfo_phase_ = 0;
    frequency_ = 2.0f;
    depth_ = 1.0f;
    waveform_ = static_cast<int>(LFO::WaveformType::kWaveformSine);
}
void Tremolo::releaseResources()
{

}
void Tremolo::processBlock(AudioBuffer<float> &buffer)
{
    const auto num_channels {buffer.getNumChannels()};
    const auto num_samples  {buffer.getNumFrames()};
    float phase{0.0f};

    for(size_t c = 0; c < num_channels; ++c)
    {
        auto* channel_data = buffer.getWritePointer(c);
        phase = impl_->lfo_phase_;

        for(size_t i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];
            channel_data[i] = in * (1.0f - depth_*impl_->lfo_.lfo(phase, static_cast<LFO::WaveformType>(waveform_)));

            phase += frequency_*impl_->invert_sample_rate_;
            if(phase >= 1.0f)
            {
                phase -= 1.0f;
            }
        }
    }

    impl_->lfo_phase_ = phase;
}

}