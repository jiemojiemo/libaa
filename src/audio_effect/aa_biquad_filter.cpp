
//
// Created by William.Hua on 2020/12/8.
//

#include "audio_effect/aa_biquad_filter.h"

namespace libaa
{
void BiquadFilter::processBlock(AudioBuffer<float> &buffer)  {
    const auto num_channels = std::min(buffer.getNumChannels(), BiquadFilter::kDefaultNumChannels);
    const auto num_frames = buffer.getNumFrames();

    for(auto c = 0u; c < num_channels; ++c){
        auto* channel = buffer.getWritePointer(c);
        for(auto i = 0u; i < num_frames; ++i){
            channel[i] = biquad_filters_[c](channel[i]);
        }
    }
}

void BiquadFilter::setCoefficients(const FilterCoeffs& coff){
    for(auto& biq : biquad_filters_){
        biq.coeff_array_ = coff;
    }
}
}