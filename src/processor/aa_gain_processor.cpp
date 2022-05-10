
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/processor/aa_gain_processor.h"
#include "libaa/aa_version.h"
#include "libaa/dsp/aa_db_utils.h"
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
    float scale = dbToScale(db);
    for (size_t c = 0; c < block->buffer.getNumberChannels(); ++c) {
        float *buffer = block->buffer.getWriterPointer(c);
        for (size_t i = 0; i < block->buffer.getNumberFrames(); ++i) {
            buffer[i] *= scale;
        }
    }
}
void GainProcessor::setState(uint8_t *state, size_t size) {
    nlohmann::json state_json = nlohmann::json::parse((char *)(state), (char *)(state + size));
    nlohmann::json &param_json = state_json["parameters"];

    for (auto it = param_json.begin(); it != param_json.end(); ++it) {
        std::string param_name = it.key();
        auto p_index = parameters.findParameterIndexByName(param_name);
        if (p_index != -1) {
            float plain_value = it.value().get<float>();
            parameters.get(p_index).setPlainValue(plain_value);
        }
    }
}
vector<uint8_t> GainProcessor::getState() const {
    nlohmann::json state_json;
    state_json["version"] = LIBAA_VERSION;
    state_json["processor_name"] = getName();
    state_json["parameters"] = {
        {getParameters()->get(0).getParameterName(), getParameters()->get(0).getPlainValue()}};

    auto state_string = to_string(state_json);
    return vector<uint8_t>{state_string.begin(), state_string.end()};
}

} // namespace libaa