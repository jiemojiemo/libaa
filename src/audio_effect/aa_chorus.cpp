//
// Created by william on 2020/3/10.
//

#include "audio_effect/aa_chorus.h"
#include "dsp/aa_lfo.h"
#include "audio_basics/aa_delay_line_array.h"

namespace libaa
{
class Chorus::Impl
{
public:
    float phase_={0.0f};
    float invert_sample_rate_{0.0f};
    LFO lfo_;
    DelayLineArray<float> dlines_;
};

Chorus::Chorus():
    impl_(std::make_shared<Impl>())
{
}

void Chorus::prepareToPlay(double sample_rate, int max_block_size)
{
    impl_->invert_sample_rate_ = 1.0/sample_rate;

    const int num_supported_channel = 2;
    const float max_delay_length = 1.0f; // 1 seconds
    const auto max_delay_length_sample = static_cast<size_t>(max_delay_length * sample_rate);

    impl_->dlines_.allocateDelayLines(num_supported_channel, max_delay_length_sample);
}
void Chorus::reset()
{

}
void Chorus::releaseResources()
{

}
void Chorus::processBlock(AudioBuffer<float> &buffer)
{
    const auto num_channels = buffer.getNumChannels();
    const auto num_samples = buffer.getNumFrames();

    float ph = 0.0f;

    for(size_t c = 0; c < num_channels; ++c)
    {
        auto* channel_data = buffer.getWritePointer(c);
        auto* dline = impl_->dlines_.getDelayLine(c);

        ph = impl_->phase_;

        for(size_t i = 0; i < num_samples; ++i)
        {
            const float in = channel_data[i];
            float weight = 0.0f;
            float phase_offset = 0.0f;

            for(int j  = 0; j < num_voices - 1; ++j)
            {
                if(stereo != 0 && num_voices > 2)
                {
                    weight = (float)(j) / (float)(num_voices-2);

                    if(c != 0)
                    {
                        weight = 1.0f - weight;
                    }

                } else
                {
                    weight = 1.0f;
                }

                if(weight != 0.0f)
                {
                    // get delay from lfo
                    float delay_second = min_delay + sweep_width*impl_->lfo_.lfo(fmodf(ph+phase_offset,1.0f), LFO::WaveformType::kWaveformSine);
                    float delay_sample = delay_second * static_cast<float>(getSampleRate());

                    // get interpolation value
                    float interpolation_val = dline->getInterpolation(delay_sample);
                    channel_data[i] = in + weight * depth * interpolation_val;
                }

                if(num_voices < 3)
                {
                    phase_offset += 0.25f;
                } else
                {
                    phase_offset += 1.0f / (float)(num_voices - 1);
                }
            }

            dline->push(in);

            // update phase
            ph += lfo_freq*impl_->invert_sample_rate_;
            if(ph >= 1.0f)
            {
                ph -= 1.0f;
            }
        }
    }

    impl_->phase_ = ph;
}

}