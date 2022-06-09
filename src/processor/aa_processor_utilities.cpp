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
bool updateParameterFromParameterChanges(ParameterChanges &param_changes,
                                         AudioProcessorParameters &param) {
    bool param_changed = false;
    if (param_changes.getNumParameters() != 0) {
        ParameterChangePoint point{0, 0, 0};
        for (size_t i = 0; i < param.size(); ++i) {
            if (param_changes.pop(i, point)) {
                param.get(i).setNormalizedValue(point.normalized_value);
                param_changed = true;
            }
        }
    }
    return param_changed;
}

std::string serializeProcessorToString(const IAudioProcessor *proc) {
    return serializeProcessorToJson(proc).dump();
}

nlohmann::json serializeProcessorToJson(const IAudioProcessor *proc) {
    nlohmann::json result;
    result["version"] = LIBAA_VERSION;
    result["processor_name"] = proc->getName();

    const auto *parameters = proc->getParameters();
    if (parameters != nullptr) {
        nlohmann::json param_json;
        for (auto i = 0u; i < parameters->size(); ++i) {
            const AudioProcessorParameter &current_param = parameters->get(i);

            auto param_type = current_param.getParameterType();
            switch (param_type) {
            case ParameterType::kFloat:
                param_json[current_param.getParameterName()] = current_param.getPlainValue();
                break;
            case ParameterType::kChoice:
                param_json[current_param.getParameterName()] = current_param.getChoiceString();
                break;
            case ParameterType::kBool:
                param_json[current_param.getParameterName()] = current_param.getBool();
                break;
            case ParameterType::kInt:
                param_json[current_param.getParameterName()] = current_param.getInt();
                break;
            }
        }
        result["parameters"] = param_json;
    }

    return result;
}

std::vector<uint8_t> serializeProcessorToBinaryArray(const IAudioProcessor *proc) {
    auto state_string = ProcessorUtilities::serializeProcessorToString(proc);
    return std::vector<uint8_t>{state_string.begin(), state_string.end()};
}

std::string convertProcessorStateToString(const std::vector<uint8_t> &state) {
    return std::string{state.begin(), state.end()};
}

void updateParametersFromState(uint8_t *state, size_t size, AudioProcessorParameters &parameters) {
    nlohmann::json state_json = nlohmann::json::parse((char *)(state), (char *)(state + size));
    updateParametersFromState(state_json, parameters);
}

void updateParametersFromState(const nlohmann::json &state_json, AudioProcessorParameters &parameters) {
    const nlohmann::json &param_json = state_json["parameters"];

    for (auto it = param_json.begin(); it != param_json.end(); ++it) {
        const std::string &param_name = it.key();
        auto p_index = parameters.findParameterIndexByName(param_name);
        if (p_index != -1) {
            auto &current_param = parameters.get(p_index);
            auto param_value_str = it.value().is_string() ? it.value().get<std::string>() : it.value().dump();
            current_param.setPlainValue(param_value_str);
        }
    }
}

} // namespace ProcessorUtilities
} // namespace libaa