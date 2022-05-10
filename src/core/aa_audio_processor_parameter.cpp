
//
// Created by William.Hua on 2021/5/19.
//
#include "libaa/core/aa_audio_processor_parameter.h"

namespace libaa {

namespace {
inline float convertBoolToFloat(bool b) {
    return b ? 1.0f : 0.0f;
}
} // namespace

AudioProcessorParameter::AudioProcessorParameter(
    ParameterType type, int param_id, std::string param_name, float default_val,
    float min_plain_value, float max_plain_value,
    std::vector<std::string> choices_strings)
    : type_(type), id_(param_id), plain_value_(default_val),
      min_plain_value_(min_plain_value), max_plain_value_(max_plain_value),
      name_(std::move(param_name)),
      choice_strings_(std::move(choices_strings)) {
    if (min_plain_value_ > max_plain_value_) {
        min_plain_value_ = max_plain_value_;
    }

    if (!isInRange(plain_value_)) {
        plain_value_ = getMinPlainValue();
    }

    current_normalized_value_ =
        convertPlainValueToNormalizedValue(plain_value_);
}

AudioProcessorParameter::AudioProcessorParameter(int param_id,
                                                 std::string param_name,
                                                 bool init_bool)
    : AudioProcessorParameter(ParameterType::kBool, param_id, param_name,
                              convertBoolToFloat(init_bool), 0.0f, 1.0f) {}

float AudioProcessorParameter::getDefaultPlainValue() const {
    return plain_value_;
}

int AudioProcessorParameter::getParameterID() const {
    return id_;
}

float AudioProcessorParameter::getMinPlainValue() const {
    return min_plain_value_;
}

float AudioProcessorParameter::getMaxPlainValue() const {
    return max_plain_value_;
}

float AudioProcessorParameter::convertPlainValueToNormalizedValue(
    float plain_value) const {
    plain_value = clip(plain_value, getMinPlainValue(), getMaxPlainValue());

    return (plain_value - getMinPlainValue()) /
           (getMaxPlainValue() - getMinPlainValue());
}

float AudioProcessorParameter::convertPlainValueStringToNormalizedValue(const std::string &plain_value_str) const {
    if (getParameterType() == ParameterType::kFloat) {
        return convertPlainValueToNormalizedValue(std::stof(plain_value_str));
    } else if (getParameterType() == ParameterType::kBool) {
        bool bool_value = (plain_value_str == "true");
        return convertBoolToFloat(bool_value);
    } else if (getParameterType() == ParameterType::kChoice) {
        auto find_or_not = std::find(choice_strings_.begin(), choice_strings_.end(), plain_value_str);
        if (find_or_not != choice_strings_.end()) {
            auto dist = std::distance(choice_strings_.begin(), find_or_not);
            return convertPlainValueToNormalizedValue(float(dist));
        }
    }

    throw std::invalid_argument("invalid plain value string");
}

float AudioProcessorParameter::convertNormalizedValueToPlainValue(
    float norm_value) const {
    norm_value = clip(norm_value, 0, 1);

    return (getMaxPlainValue() - getMinPlainValue()) * norm_value +
           getMinPlainValue();
}

std::string AudioProcessorParameter::convertNormalizedValueToChoiceString(
    float norm_value) const {
    size_t choice_index =
        static_cast<size_t>(convertNormalizedValueToPlainValue(norm_value));
    if (choice_index >= choice_strings_.size()) {
        return {};
    }
    return choice_strings_[choice_index];
}

bool AudioProcessorParameter::convertNormalizedValueToBool(
    float norm_value) const {
    return norm_value > 0.5f;
}

float AudioProcessorParameter::getPlainValue() const {
    return convertNormalizedValueToPlainValue(current_normalized_value_);
}

float AudioProcessorParameter::getNormalizedValue() const {
    return current_normalized_value_;
}

bool AudioProcessorParameter::getBool() const {
    return convertNormalizedValueToBool(current_normalized_value_);
}

int AudioProcessorParameter::getInt() const {
    return static_cast<int>(getPlainValue());
}

std::string AudioProcessorParameter::getChoiceString() const {
    return convertNormalizedValueToChoiceString(current_normalized_value_);
}

std::string AudioProcessorParameter::getPlainValueString() const {
    return std::to_string(getPlainValue());
}

void AudioProcessorParameter::setPlainValue(float plain_value) {
    current_normalized_value_ = convertPlainValueToNormalizedValue(plain_value);
}

void AudioProcessorParameter::setPlainValue(
    const std::string &plain_value_str) {
    setNormalizedValue(convertPlainValueStringToNormalizedValue(plain_value_str));
}

void AudioProcessorParameter::setNormalizedValue(float normalized_value) {
    current_normalized_value_ = clip(normalized_value, 0, 1);
}

void AudioProcessorParameter::setNormalizedValue(
    const std::string &normalized_value_str) {
    setNormalizedValue(std::stof(normalized_value_str));
}

void AudioProcessorParameter::setBoolValue(bool bool_value) {
    setNormalizedValue(convertBoolToFloat(bool_value));
}

bool AudioProcessorParameter::isInRange(float v) const {
    return getMinPlainValue() <= v && v <= getMaxPlainValue();
}

float AudioProcessorParameter::clip(float v, float min, float max) const {
    return v < min ? min : (v > max ? max : v);
}
ParameterType AudioProcessorParameter::getParameterType() const {
    return type_;
}
const std::string &AudioProcessorParameter::getParameterName() const {
    return name_;
}
void AudioProcessorParameter::setParameterName(std::string new_name) {
    name_ = std::move(new_name);
}

std::vector<std::string> AudioProcessorParameter::getChoiceStrings() const {
    return choice_strings_;
}

bool operator==(const AudioProcessorParameter &lhs,
                const AudioProcessorParameter &rhs) {
    return lhs.getPlainValue() == rhs.getPlainValue() &&
           lhs.getMinPlainValue() == rhs.getMinPlainValue() &&
           lhs.getMaxPlainValue() == rhs.getMaxPlainValue();
}
} // namespace libaa