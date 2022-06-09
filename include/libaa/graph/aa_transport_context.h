//
// Created by user on 5/16/22.
//

#ifndef LIBAA_AA_TRANSPORT_CONTEXT_H
#define LIBAA_AA_TRANSPORT_CONTEXT_H

#pragma once
#include <atomic>
namespace libaa {
class TransportContext {
public:
    TransportContext() {
        static_assert(std::atomic<float>::is_always_lock_free);
        static_assert(std::atomic<size_t>::is_always_lock_free);
    }

    std::atomic<size_t> num_samples{0};
    std::atomic<float> sample_rate{0};
    std::atomic<bool> is_playing{false};
    std::atomic<bool> is_final{false};
    std::atomic<bool> is_push_pull_mode{false};
    std::atomic<size_t> play_head_sample_index{0};
};
} // namespace libaa

#endif // LIBAA_AA_TRANSPORT_CONTEXT_H
