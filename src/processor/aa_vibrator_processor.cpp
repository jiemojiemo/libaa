//
// Created by William on 3/25/22.
//
#include "libaa/processor/aa_vibrator_processor.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"
namespace libaa {
VibratorProcessor::VibratorProcessor() {
    param_.pushFloatParameter("Rate", 0.2f, 0.1f, 20.0f);
    param_.pushFloatParameter("Depth", 0.5f, 0.0f, 1.0f);
}

void VibratorProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    utility_.prepare(sample_rate, max_block_size);
}

std::string VibratorProcessor::getName() const { return {"Vibrator"}; }
void VibratorProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, param_);

    float depth = param_.get(1).getPlainValue();
    float rate = param_.get(0).getPlainValue();

    utility_.process(&output->buffer, depth, rate);
}
const AudioProcessorParameters *VibratorProcessor::getParameters() const {
    return &param_;
}
} // namespace libaa