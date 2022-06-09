//
// Created by user on 6/9/22.
//

#include "libaa/processor/aa_resampler_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
ResamplerProcessor::ResamplerProcessor() {
    params_.pushFloatParameter("Ratio", 1.0, 0.1, 10);
}
std::string ResamplerProcessor::getName() const {
    return {"Resampler"};
}
void ResamplerProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);
    for (auto &r : resamplers_) {
        r.prepare(sample_rate, 1, max_block_size);
    }
}
void ResamplerProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    ProcessorUtilities::updateParameterFromParameterChanges(input->param_changes, params_);

    float ratio = params_.get(0).getPlainValue();
    size_t input_num_samples = input->context.num_samples;
    bool is_final = input->context.is_final;
    size_t output_buffer_size = ceil(ratio * input_num_samples);
    if (output_buffer_size > output->buffer.getNumberFrames()) {
        output->buffer.resizeFrames(output_buffer_size);
    }

    size_t output_size = 0;
    for (auto c = 0u; c < input->buffer.getNumberChannels(); ++c) {
        float *input_buffer = input->buffer.getWriterPointer(c);
        float *output_buffer = output->buffer.getWriterPointer(c);
        output_size = resamplers_[c].resampleInterleaved(input_buffer, input_num_samples, output_buffer, output_buffer_size, ratio, is_final);
    }

    output->context.num_samples = output_size;
}
const AudioProcessorParameters *ResamplerProcessor::getParameters() const {
    return &params_;
}
void ResamplerProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, params_);
}
std::vector<uint8_t> ResamplerProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}
} // namespace libaa
