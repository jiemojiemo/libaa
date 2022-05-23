
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/processor/aa_gain_processor.h"
#include "libaa/aa_version.h"
#include "libaa/dsp/aa_db_utils.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <cmath>
#include <nlohmann/json.hpp>
namespace libaa {
GainProcessor::GainProcessor(float gain_db) {
    parameters.pushFloatParameter("Gain dB", gain_db, -96.0f, 35.0f);
}
std::string GainProcessor::getName() const {
    return {"Gain"};
}

void GainProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);

    updateGainParameter(input);
    applyGain(output);
}
const AudioProcessorParameters *GainProcessor::getParameters() const {
    return &parameters;
}

void GainProcessor::updateGainParameter(AudioBlock *block) {
    if (block->param_changes.getNumParameters() != 0) {
        ParameterChangePoint point{0, 0, 0};
        if (block->param_changes.pop(0, point)) {
            parameters.get(0).setNormalizedValue(point.normalized_value);
        }
    }
}

void GainProcessor::applyGain(AudioBlock *block) {
    float db = parameters.get(0).getPlainValue();
    float scale = DbUtils::dbToScale(db);
    for (size_t c = 0; c < block->buffer.getNumberChannels(); ++c) {
        float *buffer = block->buffer.getWriterPointer(c);
        for (size_t i = 0; i < block->buffer.getNumberFrames(); ++i) {
            buffer[i] *= scale;
        }
    }
}
void GainProcessor::setState(uint8_t *state, size_t size) {
    ProcessorUtilities::updateParametersFromState(state, size, parameters);
}
vector<uint8_t> GainProcessor::getState() const {
    return ProcessorUtilities::serializeProcessorToBinaryArray(this);
}

} // namespace libaa