//
// Created by admin on 2020/7/20.
//

#include "audio_effect/aa_distortion.h"

namespace libaa
{
class Distortion::Impl
{
public:
    static float clip(const float in, int type)
    {
        float out = {0.0f};
        if(type == DistortionType::kHardClipping)
        {
            out = hardClip(in);
        }else if(type == DistortionType::kSoftClipping)
        {
            out = softClip(in);
        }else if(type == DistortionType::kSoftClippingExponential)
        {
            out = softClipExponential(in);
        }else if(type == DistortionType::kFullWaveRectifier)
        {
            out = fullWaveRectifier(in);
        }else if(type == DistortionType::kHalfWaveRectifier)
        {
            out = halfWaveRectifier(in);
        }

        return out;
    }

    static float hardClip(const float in)
    {
        float out;
        float threshold = 1.0f;
        if(in > threshold)
            out = threshold;
        else if(in < -threshold)
            out = -threshold;
        else
            out = in;

        return out;
    }

    static float softClip(const float in)
    {
        float out;
        float threshold1 = 1.0f / 3.0f;
        float threshold2 = 2.0f / 3.0f;

        if(in > threshold2)
            out = 1.0f;                                                 // [2/3, 1.0]
        else if(in > threshold1)
            out = (3.0f - (2.0f - 3.0f*in)*(2.0f - 3.0f*in))/3.0f;      // [1/3, 2/3]
        else if(in < -threshold2)
            out = -1.0f;                                                // [-1, -2/3]
        else if(in < -threshold1)
            out = -(3.0f - (2.0f - 3.0f*in)*(2.0f - 3.0f*in))/3.0f;     // [-2/3, -1/3]
        else
            out = 2.0f * in;                                            // [-1/3, 1/3]

        return out;
    }

    static float softClipExponential(const float in)
    {
        float out;

        if(in > 0.0f)
            out = 1.0f - expf(-in);
        else
            out = -1.0f + expf(in);

        return out;
    }

    static float fullWaveRectifier(const float in)
    {
        return fabs(in);
    }

    static float halfWaveRectifier(const float in)
    {
        if(in > 0)
            return in;
        else
            return -in;
    }
};

Distortion::Distortion():
    impl_(std::make_shared<Impl>())
{

}

void Distortion::prepareToPlay(double sample_rate, int max_block_size)
{
}

void Distortion::reset()
{
    input_gainDb = 0.0f;
    distortion_type = 0;
}
void Distortion::releaseResources()
{
}

void Distortion::processBlock(AudioBuffer<float> &buffer)
{
    float gain_scale = powf(10.0f, input_gainDb/20.0f);

    for(auto c = 0u; c < buffer.getNumChannels(); ++c)
    {
        auto* channel = buffer.getWritePointer(c);

        for(auto i = 0u; i < buffer.getNumFrames(); ++i)
        {
            const float in = channel[i] * gain_scale;
            channel[i] = impl_->clip(in, distortion_type);
        }
    }
}


}