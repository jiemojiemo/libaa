//
// Created by bytedance on 2021/11/30.
//

#ifndef LIBAA_AA_I_AUDIO_PROCESSOR_H
#define LIBAA_AA_I_AUDIO_PROCESSOR_H
#pragma once
#include <vector>
#include "libaa/audio_effect/aa_processor_prepare_config.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_audio_block.h"
namespace libaa
{
class IProcessor
{
public:
    virtual int prepareToPlay(ProcessorPrepareConfig config) = 0;
    virtual std::string getName() = 0;
    virtual std::vector<uint8_t> getState() = 0;
    virtual int setState(const uint8_t* data, int data_size) = 0;
    virtual int getLatencySamples() = 0;
    virtual int getTailLengthSamples() const = 0;
    virtual void reset() = 0;
    virtual const Parameters& getParameters() = 0;
    virtual int processBlock(AudioBlock* in_block,
                             AudioBlock* output_block) = 0;
};
}


#endif //LIBAA_AA_I_AUDIO_PROCESSOR_H