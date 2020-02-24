//
// Created by william on 2020/2/24.
//

#pragma once
#include "audio_basics/aa_audio_buffer.h"
#include <string>
namespace libaa
{
class AudoEffectProcessor
{
public:
    virtual std::string getName() const  = 0;
    virtual void prepareToPlay(double sample_rate, int samplers_per_block) = 0;
    virtual void reset() = 0;
    virtual void releaseResources() = 0;
    virtual void processBlock(AudioBuffer<float>& buffer) = 0;
};
}
