//
// Created by user on 6/9/22.
//

#include "libaa/processor/aa_iir_filter_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
IIRFilterProcessor::IIRFilterProcessor() {
    params_.pushChoiceParameter("Type", 0, {"LPF1", "HPF1", "LPF2", "HPF2", "BPF2", "BSF2", "APF1", "APF2", "LowShelf", "HighShelf", "NCQParaEQ", "CQParaEQ"});
    params_.pushFloatParameter("Cutoff", 1000, 20, 20000);
    params_.pushFloatParameter("Q", 0.707, 0.707, 20);
    params_.pushFloatParameter("Boost/Cut", 0, -48, 48);
}
std::string IIRFilterProcessor::getName() const {
    return {"IIRFilter"};
}
void IIRFilterProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);
    for (auto &f : filters_) {
        f.prepare(sample_rate);
    }
    updateIIRFilterParameters();
}
void IIRFilterProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    bool changed = ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, params_);
    if (changed) {
        updateIIRFilterParameters();
    }

    for (auto c = 0u; c < output->buffer.getNumberChannels(); ++c) {
        auto &f = filters_[c];
        float *channel_data = output->buffer.getWriterPointer(c);
        for (auto i = 0u; i < input->context.num_samples; ++i) {
            channel_data[i] = f.processSample(channel_data[i]);
        }
    }
}
const AudioProcessorParameters *IIRFilterProcessor::getParameters() const {
    return &params_;
}
void IIRFilterProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, params_);
}
std::vector<uint8_t> IIRFilterProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}
void IIRFilterProcessor::updateIIRFilterParameters() {
    auto iir_params = filters_[0].getParameters();
    iir_params.type = static_cast<FilterType>(params_.get(0).getInt());
    iir_params.fc = params_.get(1).getPlainValue();
    iir_params.Q = params_.get(2).getPlainValue();
    iir_params.boost_or_cut_db = params_.get(3).getPlainValue();

    for (auto &f : filters_) {
        f.updateParameters(iir_params);
    }
}
} // namespace libaa
