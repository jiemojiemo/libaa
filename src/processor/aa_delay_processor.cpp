//
// Created by agen on 3/24/22.
//
#include "libaa/processor/aa_delay_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
namespace libaa {
DelayProcessor::DelayProcessor() {
    param_.pushFloatParameter("Delay Time(ms)", 0, 0, 2000);
    param_.pushFloatParameter("Feedback", 0, 0, 1);
    param_.pushFloatParameter("Dry", 0.5, 0, 1);
    param_.pushFloatParameter("Wet", 0.5, 0, 1);
}
std::string DelayProcessor::getName() const { return {"Delay"}; }
void DelayProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    const int max_delay_samples =
        param_.get(0).getMaxPlainValue() * sample_rate;
    utilities.prepare(max_delay_samples);
}
void DelayProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, param_);

    output->buffer.copyFrom(&input->buffer);

    float delay_ms = param_.get(0).getPlainValue();
    int delay_samples = static_cast<int>(delay_ms / 1000.0f * sample_rate_);
    float feedback = param_.get(1).getPlainValue();
    float dry = param_.get(2).getPlainValue();
    float wet = param_.get(3).getPlainValue();

    utilities.process(&output->buffer, delay_samples, feedback, dry, wet);
}
const AudioProcessorParameters *DelayProcessor::getParameters() const {
    return &param_;
}

} // namespace libaa