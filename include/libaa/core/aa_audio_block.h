
//
// Created by William.Hua on 2021/8/4.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
#pragma once
#include "libaa/core/aa_audio_buffer_new.h"
#include "libaa/core/aa_parameter_changes.h"
#include "libaa/core/aa_processing_context.h"

namespace libaa {
class AudioBlock {
public:
    explicit AudioBlock();

    explicit AudioBlock(int num_channels, int num_frames, int num_param);

    explicit AudioBlock(std::list<std::vector<float>> &&init_list,
                        int num_params);

    explicit AudioBlock(std::list<std::vector<float>> &&init_list);

    void resizeFrames(size_t num_frames);

    AudioBufferX<float> buffer;
    ParameterChanges param_changes;
    ProcessingContext context;
};

} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BLOCK_H
