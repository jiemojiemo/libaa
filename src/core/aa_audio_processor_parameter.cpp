
//
// Created by William.Hua on 2021/5/19.
//
#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_H
#include "libaa/core/aa_audio_processor_parameter.h"
#endif

namespace libaa
{
AudioProcessorParameter::AudioProcessorParameter(int param_id,
                                                 float default_val, float min_plain_value, float max_plain_value):
    id_(param_id),
    plain_value_(default_val),
    min_plain_value_(min_plain_value),
    max_plain_value_(max_plain_value)
{
    if(min_plain_value_ > max_plain_value_)
    {
        min_plain_value_ = max_plain_value_;
    }

    if(!isInRange(plain_value_))
    {
        plain_value_ = getMinPlainValue();
    }

    current_normalized_value_ = convertPlainValueToNormalizedValue(plain_value_);
}

float AudioProcessorParameter::getDefaultPlainValue() const
{
    return plain_value_;
}

int AudioProcessorParameter::getParameterID() const
{
    return id_;
}

float AudioProcessorParameter::getMinPlainValue() const
{
    return min_plain_value_;
}

float AudioProcessorParameter::getMaxPlainValue() const
{
    return max_plain_value_;
}

float AudioProcessorParameter::convertPlainValueToNormalizedValue(float plain_value) const
{
    plain_value = clip(plain_value, getMinPlainValue(), getMaxPlainValue());

    return (plain_value - getMinPlainValue()) / (getMaxPlainValue() - getMinPlainValue());
}

float AudioProcessorParameter::convertNormalizedValueToPlainValue(float norm_value) const
{
    norm_value = clip(norm_value, 0, 1);

    return (getMaxPlainValue() - getMinPlainValue()) * norm_value + getMinPlainValue();
}

float AudioProcessorParameter::getPlainValue() const
{
    return convertNormalizedValueToPlainValue(current_normalized_value_);
}

float AudioProcessorParameter::getNormalizedValue() const
{
    return current_normalized_value_;
}

std::string AudioProcessorParameter::getPlainValueString() const
{
    return std::to_string(getPlainValue());
}

void AudioProcessorParameter::setPlainValue(float plain_value)
{
    current_normalized_value_ = convertPlainValueToNormalizedValue(plain_value);
}

void AudioProcessorParameter::setPlainValue(const std::string& plain_value_str)
{
    setPlainValue(std::stof(plain_value_str));
}

void AudioProcessorParameter::setNormalizedValue(float normalized_value)
{
    current_normalized_value_ = clip(normalized_value, 0, 1);
}

void AudioProcessorParameter::setNormalizedValue(const std::string& normalized_value_str)
{
    setNormalizedValue(std::stof(normalized_value_str));
}

bool AudioProcessorParameter::isInRange(float v) const
{
    return getMinPlainValue() <= v && v <= getMaxPlainValue();
}

float AudioProcessorParameter::clip(float v, float min, float max) const
{
    return  v < min ? min : (v > max ? max : v);
}

bool operator==(const AudioProcessorParameter &lhs, const AudioProcessorParameter &rhs) {
    return lhs.getPlainValue() == rhs.getPlainValue()
        && lhs.getMinPlainValue() == rhs.getMinPlainValue()
        && lhs.getMaxPlainValue() == rhs.getMaxPlainValue();
}
}