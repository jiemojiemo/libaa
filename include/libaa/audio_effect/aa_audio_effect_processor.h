//
// Created by william on 2020/2/24.
//

#pragma once
#include "libaa/core/aa_audio_buffer.h"
#include <string>
namespace libaa {
class AudioEffectProcessor {
public:
    virtual ~AudioEffectProcessor() = default;
    /**
     * Returns the processor name
     */
    virtual std::string getName() const = 0;

    /**
     * Called before processing starts, to let processor prepare itself.
     */
    virtual void prepareToPlay(double sample_rate, int max_block_size) = 0;

    /**
     * Resets internal states.
     */
    virtual void reset() = 0;

    /**
     * Called after processing has stopped, to let the object free up any
     * resources it no longer needs.
     */
    virtual void releaseResources() = 0;

    /**
     * Renders the next block.
     */
    virtual void processBlock(AudioBuffer<float> &buffer) = 0;

    /**
     * Returns the current sample rate
     */
    double getSampleRate() const noexcept;

    /**
     * Returns the current block size
     */
    int getBlockSize() const noexcept;

    /**
     * Returns number of samples, the processor's latency
     */
    int getLatency() const noexcept {
        return 0;
    }

private:
    double sample_rate_{0.0};
    int block_size_{0};
};

} // namespace libaa
