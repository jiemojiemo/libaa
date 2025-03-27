//
// Created by user on 6/14/22.
//

#include "libaa/processor/aa_time_stretching_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

namespace libaa {
std::string TimeStretchingProcessor::getName() const {
    return {"TimeStretching"};
}

void TimeStretchingProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    utilities_.prepare(sample_rate, kMaxChannels, max_block_size);
}

void TimeStretchingProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    updateUtilitiesParameters(input->param_changes);

    push(input);

    retrieve(output);
}

void TimeStretchingProcessor::push(AudioBlock *input) {
    // push audio
    data_refer_to_[0] = input->buffer.getWriterPointer(0);
    buildDataReferFromBlock(input);
    utilities_.process(data_refer_to_, input->context.num_samples, input->context.is_final);
}

void TimeStretchingProcessor::retrieve(AudioBlock *output) {
    // retrieve data
    int output_size = utilities_.available();
    if (output_size > 0) {
        if (output_size > static_cast<int>(output->buffer.getNumberFrames())) {
            output->buffer.resizeFrames(output_size);
        }

        buildDataReferFromBlock(output);

        utilities_.retrieve(data_refer_to_, output_size);

        output->context.num_samples = output_size;
    } else {
        output->context.num_samples = 0;
    }
}

void TimeStretchingProcessor::buildDataReferFromBlock(AudioBlock *block) {
    auto num_in_channels = block->buffer.getNumberChannels();
    data_refer_to_[0] = block->buffer.getWriterPointer(0);
    if (num_in_channels > 1) {
        data_refer_to_[1] = block->buffer.getWriterPointer(1);
    } else {
        data_refer_to_[1] = data_refer_to_[0];
    }
}

const AudioProcessorParameters *TimeStretchingProcessor::getParameters() const {
    return utilities_.getParameters();
}
void TimeStretchingProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, *utilities_.getParameters());
}
std::vector<uint8_t> TimeStretchingProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

void TimeStretchingProcessor::updateUtilitiesParameters(ParameterChanges &param_changes) {
    if (param_changes.getNumParameters() != 0) {
        ParameterChangePoint point{0, 0, 0};
        auto num_param = utilities_.getParameters()->size();
        for (size_t i = 0; i < num_param; ++i) {
            if (param_changes.pop(i, point)) {
                utilities_.pushParameterChangePoint(point);
            }
        }
    }
}

} // namespace libaa