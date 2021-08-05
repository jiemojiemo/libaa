
//
// Created by William.Hua on 2021/5/19.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_H
#pragma once
#include <string>
namespace libaa
{
enum class ParameterType
{
    kFloat = 0,
};

class AudioProcessorParameter
{
public:
    explicit AudioProcessorParameter(ParameterType type, int param_id,
                                     std::string param_name,
                                     float default_val, float min_plain_value, float max_plain_value);

    float getDefaultPlainValue() const;

    ParameterType getParameterType() const;

    int getParameterID() const;

    float getMinPlainValue() const;

    float getMaxPlainValue() const;

    float convertPlainValueToNormalizedValue(float plain_value) const;

    float convertNormalizedValueToPlainValue(float norm_value) const;

    float getPlainValue() const;

    float getNormalizedValue() const;

    std::string getPlainValueString() const;

    void setPlainValue(float plain_value);

    void setPlainValue(const std::string& plain_value_str);

    void setNormalizedValue(float normalized_value);

    void setNormalizedValue(const std::string& normalized_value_str);

    const std::string& getParameterName() const;

    void setParameterName(std::string new_name);

private:
    bool isInRange(float v) const;

    float clip(float v, float min, float max) const;

private:
    ParameterType type_{ParameterType::kFloat};
    int id_{0};
    float plain_value_{0};
    float min_plain_value_{0};
    float max_plain_value_{0};
    float current_normalized_value_{0};
    std::string name_;
};

bool operator==(const AudioProcessorParameter& lhs, const AudioProcessorParameter& rhs);

}

#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_H
