//
// Created by agen on 4/11/22.
//

#include "libaa/processor/aa_pitch_shifter_utilities.h"

#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_pitch_shifter_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
class PitchShiftProcessor::Impl {
public:
    AudioProcessorParameters params_;
    std::unique_ptr<PitchShifterUtilities> utilities_{nullptr};
    std::unique_ptr<PitchShifterUtilities> utilities_stereo{nullptr};
};

PitchShiftProcessor::PitchShiftProcessor()
    : impl_(std::make_shared<Impl>()) {
    impl_->params_.pushFloatParameter("Octaves", 0, -2.0f, 2.0f);
    impl_->params_.pushFloatParameter("Semitones", 0, -12.0f, 12.0f);
    impl_->params_.pushFloatParameter("Cents", 0, -100.0f, 100.0f);
}
std::string PitchShiftProcessor::getName() const {
    return {"PitchShifter"};
}
void PitchShiftProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    // TODO: improve this
    impl_->utilities_ = std::make_unique<PitchShifterUtilities>(sample_rate, 1);
    impl_->utilities_stereo =
        std::make_unique<PitchShifterUtilities>(sample_rate, 2);
}
void PitchShiftProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);
    ProcessorUtilities::updateParameterFromParameterChanges(
        input->param_changes, impl_->params_);

    size_t num_channels = output->buffer.getNumberChannels();
    PitchShifterUtilities *u = nullptr;
    if (num_channels == 1) {
        u = impl_->utilities_.get();
    } else if (num_channels == 2) {
        u = impl_->utilities_stereo.get();
    }

    // utilities update parameters
    float octaves_value = impl_->params_.get(0).getPlainValue();
    float semitones_value = impl_->params_.get(1).getPlainValue();
    float cents_value = impl_->params_.get(2).getPlainValue();
    u->m_octaves = &octaves_value;
    u->m_semitones = &semitones_value;
    u->m_cents = &cents_value;

    for (auto c = 0u; c < output->buffer.getNumberChannels(); ++c) {
        u->m_input[c] = output->buffer.getWriterPointer(c);
        u->m_output[c] = output->buffer.getWriterPointer(c);
    }

    u->runImpl(output->buffer.getNumberFrames());
}
const AudioProcessorParameters *PitchShiftProcessor::getParameters() const {
    return &impl_->params_;
}

int PitchShiftProcessor::getLatencySamples() const noexcept {
    if (impl_->utilities_ != nullptr) {
        return impl_->utilities_->getLatency();
    }
    return -1;
}

void *PitchShiftProcessor::getPitchShifterImpl() const {
    return impl_->utilities_.get();
}
} // namespace libaa