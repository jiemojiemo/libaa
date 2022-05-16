//
// Created by user on 3/24/22.
//

#ifndef LIBAA_AA_AUDIO_PROCESSOR_H
#define LIBAA_AA_AUDIO_PROCESSOR_H

#pragma once
#include "libaa/audio_effect/aa_processor_prepare_config.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include <vector>
namespace libaa {
class AudioProcessorParameters;
class AudioBlock;
class IAudioProcessor {
public:
    virtual ~IAudioProcessor() = default;

    /**
     * Returns the processor name
     */
    virtual std::string getName() const = 0;

    /**
     * Called before processing starts, to let processor prepare itself.
     */
    virtual void prepareToPlay(float sample_rate, int max_block_size) {
        sample_rate_ = sample_rate;
        max_block_size_ = max_block_size;
    }

    /**
     * Renders next block
     */
    virtual void processBlock(AudioBlock *input, AudioBlock *output) = 0;

    /**
     * Returns the sample rate which set in prepareToPlay()
     */
    double getSampleRate() const noexcept {
        return sample_rate_;
    }

    /**
     * Returns the max block size which set in prepareToPlay()
     */
    int getMaxBlockSize() const noexcept {
        return max_block_size_;
    }

    /**
     * Returns the processing latency in samples
     */
    virtual int getLatencySamples() const noexcept {
        return 0;
    }

    /**
     * Returns the length of the processor's tail, in seconds.
     */
    virtual float getTailLengthSeconds() const {
        return 0.0f;
    }

    virtual int getTailLengthSamples() const {
        return 0;
    }

    /**
     * Returns processor parameters, returns nullptr if there has no parameters
     */
    virtual const AudioProcessorParameters *getParameters() const = 0;

    /**
     * Sets processor state from binary data, @see IAudioProcessor::getState
     * @param state the binary data array
     * @param size the binary data size
     */
    virtual void setState(uint8_t *state, size_t size) = 0;

    /**
     * Returns the processor state, serialize processor to binary data, @see IAudioProcessor::setState
     */
    virtual std::vector<uint8_t> getState() const = 0;

protected:
    float sample_rate_{0.0f};
    int max_block_size_{0};
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_PROCESSOR_H
