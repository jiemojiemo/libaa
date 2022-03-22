
//
// Created by William.Hua on 2021/8/4.
//

#include "libaa/core/aa_audio_block.h"

namespace libaa {
AudioBlock::AudioBlock() : AudioBlock(0, 0) {}

AudioBlock::AudioBlock(int num_channels, int num_frames)
    : audio_buffer(num_channels, num_frames) {}

AudioBlock::AudioBlock(int num_channels, int num_frames, int num_params)
    : audio_buffer(num_channels, num_frames), param_changes(num_params) {}

AudioBlock::AudioBlock(std::list<std::vector<float>> &&init_list)
    : audio_buffer(std::move(init_list)) {}

AudioBlock::AudioBlock(AudioBufferNew<float> buffer, ParameterChanges changes)
    : audio_buffer(std::move(buffer)), param_changes(std::move(changes)) {}

} // namespace libaa