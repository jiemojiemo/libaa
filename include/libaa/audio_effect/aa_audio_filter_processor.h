//
// Created by hw on 2021/12/11.
//

#ifndef LIBAA_AA_AUDIO_FILTER_PROCESSOR_H
#define LIBAA_AA_AUDIO_FILTER_PROCESSOR_H
#include "libaa/audio_effect/aa_i_audio_processor.h"
#include "libaa/dsp/aa_audio_filter_utilities.h"
namespace libaa {
class AudioFilterProcessor : public IProcessor {
public:
    AudioFilterProcessor();

    int prepareToPlay(ProcessorPrepareConfig config) override;

    std::string getName() override;

    std::vector<uint8_t> getState() override;

    int setState(const uint8_t *data, int data_size) override;

    int getLatencySamples() override;

    int getTailLengthSamples() const override;

    void reset() override;

    const Parameters &getParameters() override;

    int processBlock(AudioBlock *in_block, AudioBlock *output_block) override;

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_FILTER_PROCESSOR_H
