//
// Created by agen on 3/25/22.
//

#ifndef AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H
#define AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H

#pragma once

namespace libaa {
class ParameterChanges;
class AudioProcessorParameters;
namespace ProcessorUtilities {
void updateParameterFromParameterChanges(ParameterChanges &param_changes,
                                         AudioProcessorParameters &param);
} // namespace ProcessorUtilities

} // namespace libaa

#endif // AUDIOEFFECT_AE_PROCESSOR_UTILITIES_H
