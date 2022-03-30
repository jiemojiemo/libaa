//
// Created by William on 3/25/22.
//

#include "libaa/processor/aa_processor_utilities.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_parameter_changes.h"
namespace libaa {
class ParameterChanges;
class AudioProcessorParameters;
namespace ProcessorUtilities {
void updateParameterFromParameterChanges(ParameterChanges &param_changes,
                                         AudioProcessorParameters &param) {
    if (param_changes.getNumParameters() != 0) {
        ParameterChangePoint point{0, 0, 0};
        for (size_t i = 0; i < param.size(); ++i) {
            if (param_changes.pop(i, point)) {
                param.get(i).setNormalizedValue(point.normalized_value);
            }
        }
    }
}
} // namespace ProcessorUtilities
} // namespace libaa