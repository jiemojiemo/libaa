
//
// Created by William.Hua on 2021/5/19.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_FLOAT_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_FLOAT_H
#pragma once
#include <string>
namespace libaa
{
class AudioProcessorParameterFloat
{
public:
    explicit AudioProcessorParameterFloat(int param_id,
                                          float default_val,float min_plain_value, float max_plain_value);

    float getDefaultPlainValue() const;

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

private:
    bool isInRange(float v) const;

    float clip(float v, float min, float max) const;

private:
    int id_{0};
    float plain_value_{0};
    float min_plain_value_{0};
    float max_plain_value_{0};
    float current_normalized_value_{0};
};

}

#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_PROCESSOR_PARAMETER_FLOAT_H
