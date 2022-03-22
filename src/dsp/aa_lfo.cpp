//
// Created by william on 2020/2/26.
//

#include "libaa/dsp/aa_lfo.h"
namespace libaa {
float LFO::lfo(float phase, WaveformType waveform) {
    if (waveform == WaveformType::kWaveformSine) {
        return lfoSine(phase);
    }
    return lfoSine(phase);
}

float LFO::lfoSine(float phase) const {
    return 0.5f + 0.5f * sinf(2 * M_PI * phase);
}
} // namespace libaa