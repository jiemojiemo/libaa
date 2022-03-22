//
// Created by bytedance on 2021/11/30.
//

#ifndef LIBAA_AA_PROCESSOR_PREPARE_CONFIG_H
#define LIBAA_AA_PROCESSOR_PREPARE_CONFIG_H
#pragma once

namespace libaa {
struct ProcessorPrepareConfig {
    float sample_rate;
    int max_block_size;
};
} // namespace libaa

#endif // LIBAA_AA_PROCESSOR_PREPARE_CONFIG_H
