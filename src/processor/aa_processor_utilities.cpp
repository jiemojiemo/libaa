//
// Created by William on 3/25/22.
//

#include "libaa/processor/aa_processor_utilities.h"
#include "libaa/aa_version.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_parameter_changes.h"
#include "libaa/processor/aa_audio_processor.h"
#include <nlohmann/json.hpp>
namespace libaa {
class ParameterChanges;
class AudioProcessorParameters;
namespace ProcessorUtilities {
void updateParameterFromParameterChanges(ParameterChanges &param_changes,
                                         AudioProcessorParameters &param) {
    if (param_changes.getNumParameters() != 0) {
        ParameterChangePoint point{0, 0, 0};
        for (size_t i = 0; i < param.size(); ++i) {
            if (param_changes.pop(i, point)) {
                param.get(i).setNormalizedValue(point.normalized_value);
            }
        }
    }
}

std::string serializeProcessorToString(const IAudioProcessor *proc) {
    nlohmann::json result;
    result["version"] = LIBAA_VERSION;
    result["processor_name"] = proc->getName();
    result["parameters"] = {
        {proc->getParameters()->get(0).getParameterName(), proc->getParameters()->get(0).getPlainValue()}};

    return result.dump();
}

std::vector<uint8_t> serializeProcessorToBinaryArray(const IAudioProcessor *proc) {
    auto state_string = ProcessorUtilities::serializeProcessorToString(proc);
    return std::vector<uint8_t>{state_string.begin(), state_string.end()};
}

std::string convertProcessorStateToString(const std::vector<uint8_t> &state) {
    return std::string{state.begin(), state.end()};
}

void updateParametersFromState(uint8_t* state, size_t size, AudioProcessorParameters& parameters){
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

} // namespace ProcessorUtilities
} // namespace libaa