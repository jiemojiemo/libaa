//
// Created by user on 5/16/22.
//

#ifndef LIBAA_AA_PROCESSING_CONTEXT_H
#define LIBAA_AA_PROCESSING_CONTEXT_H

#pragma once
#include <stddef.h>
namespace libaa {
struct ProcessingContext {
    size_t num_samples{0};
    float sample_rate;
    size_t play_head_sample_index{0};
};
} // namespace libaa

#endif // LIBAA_AA_PROCESSING_CONTEXT_H
