//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_AUDIO_PORT_H
#define LIBAA_AA_AUDIO_PORT_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include <cassert>
#include <memory>

namespace libaa {
class AudioPort {
public:
    explicit AudioPort()
        : AudioPort(nullptr, 0) {}
    explicit AudioPort(const std::shared_ptr<AudioBlock> &block)
        : AudioPort(block, block->buffer.getNumberChannels()) {}

    explicit AudioPort(std::shared_ptr<AudioBlock> block, size_t num_channels)
        : block_(std::move(block)), num_channels_(num_channels) {}

    explicit AudioPort(std::shared_ptr<AudioBlock> block, size_t num_channels,
                       size_t channel_offset)
        : block_(std::move(block)), num_channels_(num_channels),
          channel_offset_(channel_offset) {}

    float *getChannelData(int c) {
        auto remap_c = c + channel_offset_;
        if (block_ == nullptr) {
            return nullptr;
        }
        return block_->buffer.getWriterPointer(remap_c);
    }
    const float *getChannelData(int c) const {
        auto remap_c = c + channel_offset_;
        if (remap_c >= getNumberChannels() || block_ == nullptr) {
            return nullptr;
        }
        return block_->buffer.getWriterPointer(remap_c);
    }

    size_t getNumberChannels() const {
        return num_channels_;
    }

    size_t getNumberFrames() const {
        if (block_ != nullptr) {
            return block_->buffer.getNumberFrames();
        }
        return 0;
    }

    void copyFrom(const AudioPort *other) {
        auto num_samples = other->getNumberFrames();
        assert(num_samples <= getNumberFrames());
        for (auto c = 0u; c < getNumberChannels(); ++c) {
            const float *src = other->getChannelData(c);
            float *dst = getChannelData(c);

            std::copy_n(src, num_samples, dst);
        }
    }

private:
    std::shared_ptr<AudioBlock> block_{nullptr};
    size_t num_channels_{0};
    size_t channel_offset_{0};
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_PORT_H
