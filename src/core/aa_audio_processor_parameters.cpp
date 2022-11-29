
//
// Created by William.Hua on 2021/7/19.
//

#include "libaa/core/aa_audio_processor_parameters.h"

namespace libaa {
const AudioProcessorParameter &
AudioProcessorParameters::get(int param_id) const {
    return parameters_.at(param_id);
}
AudioProcessorParameter &AudioProcessorParameters::get(int param_id) {
    return parameters_.at(param_id);
}

const AudioProcessorParameter &AudioProcessorParameters::get(const std::string &param_name) const {
    return get(findParameterIndexByName(param_name));
}
AudioProcessorParameter &AudioProcessorParameters::get(const std::string &param_name) {
    return get(findParameterIndexByName(param_name));
}

int AudioProcessorParameters::pushFloatParameter(std::string param_name,
                                                 float default_val,
                                                 float min_plain_value,
                                                 float max_plain_value) {
    int new_param_index = parameters_.size();
    parameters_.emplace_back(ParameterType::kFloat, new_param_index,
                             std::move(param_name), default_val,
                             min_plain_value, max_plain_value);

    return new_param_index;
}

int AudioProcessorParameters::pushChoiceParameter(
    std::string param_name, int default_index,
    std::vector<std::string> choice_strings) {
    int new_param_index = parameters_.size();
    parameters_.emplace_back(ParameterType::kChoice, new_param_index,
                             std::move(param_name), default_index, 0,
                             choice_strings.size() - 1, std::move(choice_strings));

    return new_param_index;
}

int AudioProcessorParameters::pushBoolParameter(std::string param_name,
                                                bool default_val) {
    int new_param_index = parameters_.size();
    parameters_.emplace_back(new_param_index, std::move(param_name),
                             default_val);

    return new_param_index;
}

int AudioProcessorParameters::pushIntParameter(std::string param_name, int default_val,
                                               int min_plain_value, int max_plain_value) {
    int new_param_index = parameters_.size();
    parameters_.emplace_back(ParameterType::kInt, new_param_index,
                             std::move(param_name), default_val,
                             min_plain_value, max_plain_value);
    return new_param_index;
}

int AudioProcessorParameters::findParameterIndexByName(const std::string &param_name) const {
    for (auto i = 0u; i < parameters_.size(); ++i) {
        if (get(i).getParameterName() == param_name) {
            return i;
        }
    }
    return -1;
}

} // namespace libaa