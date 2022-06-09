//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AA_RESAMPLER_PROCESSOR_H
#define LIBAA_AA_RESAMPLER_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/dsp/aa_resampler.h"
#include "libaa/processor/aa_audio_processor.h"
#include <array>
#include <cmath>
namespace libaa {
/**
 * Resampler IS NOT realtime-safe processor, it will reallocate output.buffer's size if there is enough buffer to hold output
 * the output.context.num_samples is the number of output frames, you can use it to retrieve output data.
 *
 * @note remember to set input.context.playing = false in the final block
 *
 * If you put ResamplerProcessor on a graph, make sure the max_block_size >= ceil(ratio * block_size)
 *
 */
class ResamplerProcessor : public IAudioProcessor {
public:
    ResamplerProcessor();

    ~ResamplerProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    void setState(uint8_t *state, size_t size) override;

    std::vector<uint8_t> getState() const override;

private:
    AudioProcessorParameters params_;
    std::array<Resampler, 2> resamplers_;
};
} // namespace libaa

#endif // LIBAA_AA_RESAMPLER_PROCESSOR_H
