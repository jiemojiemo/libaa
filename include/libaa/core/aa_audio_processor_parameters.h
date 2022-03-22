
//
// Created by William.Hua on 2021/7/19.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
#pragma once
#include "libaa/core/aa_audio_processor_parameter.h"
#include <vector>
namespace libaa {
class Parameters {
public:
    Parameters() = default;

    size_t size() const { return parameters_.size(); }

    int pushFloatParameter(std::string param_name, float default_val,
                           float min_plain_value, float max_plain_value);

    int pushChoiceParameter(std::string param_name, int default_index,
                            std::vector<std::string> choice_strings);

    const AudioProcessorParameter &get(int param_id) const;
    AudioProcessorParameter &get(int param_id);

private:
    std::vector<AudioProcessorParameter> parameters_;
};

} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
