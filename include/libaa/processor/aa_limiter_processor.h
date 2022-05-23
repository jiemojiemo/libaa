//
// Created by user on 5/23/22.
//

#ifndef LIBAA_AA_LIMITER_PROCESSOR_H
#define LIBAA_AA_LIMITER_PROCESSOR_H

#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_dynamic_processor_utilities.h"

#include <array>
namespace libaa {
class LimiterProcessor : public IAudioProcessor {
public:
    LimiterProcessor();

    ~LimiterProcessor() override = default;

    std::string getName() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void processBlock(AudioBlock *input, AudioBlock *output) override;

    const AudioProcessorParameters *getParameters() const override;

    void setState(uint8_t *state, size_t size) override;

    std::vector<uint8_t> getState() const override;

    //** just for testing **
    const std::array<DynamicProcessorUtilities, 2> &getUtilities() const;

private:
    void updateDynamicProcessorUtilitiesParameters();
    AudioProcessorParameters params_;
    std::array<DynamicProcessorUtilities, 2> utilities_;
};
} // namespace libaa

#endif // LIBAA_AA_LIMITER_PROCESSOR_H
