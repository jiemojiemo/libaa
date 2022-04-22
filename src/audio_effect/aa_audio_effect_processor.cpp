//
// Created by william on 2020/2/28.
//

#include "libaa/audio_effect/aa_audio_effect_processor.h"

namespace libaa {
double AudioEffectProcessor::getSampleRate() const noexcept {
    return sample_rate_;
}
int AudioEffectProcessor::getBlockSize() const noexcept {
    return block_size_;
}
} // namespace libaa