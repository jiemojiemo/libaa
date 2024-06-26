
//
// Created by William.Hua on 2021/8/4.
//

#include "libaa/core/aa_audio_block.h"

namespace libaa {

AudioBlock::AudioBlock()
    : param_changes(0, 1) {}

AudioBlock::AudioBlock(int num_channels, int num_frames, int num_param)
    : buffer(num_channels, num_frames), param_changes(num_param, 1) {
    context.num_samples = buffer.getNumberFrames();
}

AudioBlock::AudioBlock(std::list<std::vector<float>> &&init_list,
                       int num_params)
    : buffer(std::move(init_list)), param_changes(num_params, 1) {
    context.num_samples = buffer.getNumberFrames();
}

AudioBlock::AudioBlock(std::list<std::vector<float>> &&init_list)
    : AudioBlock(std::move(init_list), 0) {
}

void AudioBlock::resizeFrames(size_t num_frames) {
    buffer.resizeFrames(num_frames);
    context.num_samples = num_frames;
}

} // namespace libaa