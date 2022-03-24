
//
// Created by William.Hua on 2021/8/4.
//

#include "libaa/core/aa_audio_block.h"

namespace libaa {
AudioBlock::AudioBlock() : param_changes(0, 1) {}

AudioBlock::AudioBlock(std::list<std::vector<float>> &&init_list,
                       int num_params)
    : buffer(std::move(init_list)), param_changes(num_params, 1) {}

AudioBlock::AudioBlock(std::list<std::vector<float>> &&init_list)
    : buffer(std::move(init_list)), param_changes(0, 1) {}

} // namespace libaa