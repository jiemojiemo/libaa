//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_AUDIO_PORT_H
#define LIBAA_AA_AUDIO_PORT_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include <memory>

namespace libaa {
class AudioPort {
public:
    explicit AudioPort(const std::shared_ptr<AudioBlock> &block)
        : AudioPort(block, block->buffer.getNumberChannels()) {}

    explicit AudioPort(std::shared_ptr<AudioBlock> block, size_t num_channels)
        : block_(std::move(block)), num_channels_(num_channels) {}

    float *getChannelData(int c) {
        if (c >= getNumberChannels()) {
            return nullptr;
        }
        return block_->buffer.getWriterPointer(c);
    }
    const float *getChannelData(int c) const {
        if (c >= getNumberChannels()) {
            return nullptr;
        }
        return block_->buffer.getWriterPointer(c);
    }

    size_t getNumberChannels() const {
        return num_channels_;
    }

    size_t getNumberFrames() const {
        return block_->buffer.getNumberFrames();
    }

private:
    std::shared_ptr<AudioBlock> block_{nullptr};
    size_t num_channels_{0};
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_PORT_H
