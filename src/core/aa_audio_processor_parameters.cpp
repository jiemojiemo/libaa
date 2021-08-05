
//
// Created by William.Hua on 2021/7/19.
//

#include "libaa/core/aa_audio_processor_parameters.h"

namespace libaa
{
const AudioProcessorParameter& Parameters::get(int param_id)const{
    return parameters_.at(param_id);
}
AudioProcessorParameter &Parameters::get(int param_id) {
    return parameters_.at(param_id);
}

int Parameters::pushFloatParameter(std::string param_name, float default_val, float min_plain_value, float max_plain_value) {
    int new_param_index = parameters_.size();
    parameters_.emplace_back(ParameterType::kFloat, new_param_index, std::move(param_name), default_val, min_plain_value, max_plain_value);

    return new_param_index;
}

}