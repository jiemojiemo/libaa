
//
// Created by William.Hua on 2021/7/19.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
#pragma once
#include "libaa/core/aa_audio_processor_parameter.h"
#include <list>
namespace libaa
{
class Parameters
{
public:
    Parameters() = default;

    size_t size() const
    {
        return parameters_list.size();
    }

    void addFloatParameter(int param_id, float default_val,float min_plain_value, float max_plain_value);

    const AudioProcessorParameter* get(int param_id) const;
private:
    void eraseParameterIfExist(int param_id);
    std::list<AudioProcessorParameter>::const_iterator findParameter(int param_id) const;

private:
    std::list<AudioProcessorParameter> parameters_list;
};

}

#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETERS_H
