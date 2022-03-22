
//
// Created by William.Hua on 2021/8/4.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
#pragma once
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_parameter_changes.h"
namespace libaa {
class AudioBlock {
public:
    explicit AudioBlock();

    explicit AudioBlock(int num_channels, int num_frames);

    explicit AudioBlock(int num_channels, int num_frames, int num_params);

    explicit AudioBlock(std::list<std::vector<float>> &&init_list);

    explicit AudioBlock(AudioBufferNew<float> buffer, ParameterChanges changes);

    AudioBufferNew<float> audio_buffer;
    ParameterChanges param_changes;
};

} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
