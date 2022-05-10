//
// Created by William on 3/25/22.
//

#ifndef AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H
#define AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H

#pragma once
#include <string>
#include <vector>
namespace libaa {
class ParameterChanges;
class AudioProcessorParameters;
class IAudioProcessor;
namespace ProcessorUtilities {
void updateParameterFromParameterChanges(ParameterChanges &param_changes,
                                         AudioProcessorParameters &param);

std::string serializeProcessorToString(const IAudioProcessor *proc);
std::vector<uint8_t> serializeProcessorToBinaryArray(const IAudioProcessor *proc);
std::string convertProcessorStateToString(const std::vector<uint8_t> &state);
void updateParametersFromState(uint8_t *state, size_t size, AudioProcessorParameters &parameters);
} // namespace ProcessorUtilities

} // namespace libaa

#endif // AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H
