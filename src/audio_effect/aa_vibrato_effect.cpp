//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_vibrato_effect.h"
#include "audio_basics/aa_delay_line_array.h"

namespace libaa
{
class VibratoEffect::Impl
{
public:
    float phase_={0.0f};
    float invert_sample_rate_{0.0f};
    LFO lfo_;
    DelayLineArray<float> dlines_;
};


void VibratoEffect::prepareToPlay(double sample_rate, int max_block_size)
{
    impl_->invert_sample_rate_ = 1.0/sample_rate;

    const int num_supported_channel = 2;
    const float max_delay_length = 2.0f; // 2 seconds
    const auto max_delay_length_sample = static_cast<size_t>(max_delay_length * sample_rate);

    impl_->dlines_.allocateDelayLines(num_supported_channel, max_delay_length_sample);

}

void VibratoEffect::processBlock(AudioBuffer<float> &buffer) {
    const int num_channels = buffer.getNumChannels();
    const int num_samples = buffer.getNumFrames();
    float phase = 0.0f;

    assert(static_cast<size_t>(num_channels) <= impl_->dlines_.getNumLines());

    for(int c = 0; c < num_channels; ++c)
    {
        phase = impl_->phase_;
        float* channel_data = buffer.getWritePointer(c);
        auto* dline = impl_->dlines_.getDelayLine(c);

        for(int i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];

            // get delay from lfo
            float delay_second = sweep_width*impl_->lfo_.lfo(phase, LFO::WaveformType::kWaveformSine);
            float delay_sample = delay_second * getSampleRate();

            // get interpolation delay value
            channel_data[i] = dline->getInterpolation(delay_sample);

            // push input to delay line
            dline->push(in);

            // update phase
            phase += lfo_freq*impl_->invert_sample_rate_;
            if(phase >= 1.0f)
            {
                phase -= 1.0f;
            }
        }
    }

    impl_->phase_ = phase;
}
VibratoEffect::VibratoEffect()
    : impl_(std::make_shared<Impl>())
{

}

}