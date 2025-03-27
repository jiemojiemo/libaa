//
// Created by user on 11/30/22.
//

#ifndef AUDIOEFFECT_AE_LOUDNESS_NORMALIZATION_PROCESSOR_H
#define AUDIOEFFECT_AE_LOUDNESS_NORMALIZATION_PROCESSOR_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/dsp/aa_db_utils.h"
#include "libaa/mir/aa_ebur128_utilities.h"
#include "libaa/mir/aa_streaming_moving_average.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_limiter_processor_utilities.h"

namespace libaa {
class LoudnessNormalizationProcessor : public IAudioProcessor {
public:
    LoudnessNormalizationProcessor();

    void prepareToPlay(float sample_rate, int max_block_size) override;

    std::string getName() const override;
    void processBlock(AudioBlock *input, AudioBlock *output) override;
    const AudioProcessorParameters *getParameters() const override;
    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    void prepareBuffersForProcessing(AudioBlock *output, int num_channels);
    void offlineLoudnessProcessing(AudioBlock *output, float target_lufs, float target_peak);
    void liveLoudnessProcessing(AudioBlock *output, float target_lufs, float target_peak);
    float calcSmoothGainDb(float in_db);
    AudioProcessorParameters params_;
    MIR::Ebur128Utilities meter_;
    std::vector<float *> data_refer_to_;
    LimiterProcessorUtilities limiter_utilities_;
    MIR::StreamingMovingAverage moving_average_;
    constexpr static int kNumChannels = 2;
};
} // namespace libaa

#endif // AUDIOEFFECT_AE_LOUDNESS_NORMALIZATION_PROCESSOR_H
