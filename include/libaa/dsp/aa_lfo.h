//
// Created by william on 2020/2/26.
//

#pragma once
#include "libaa/aa_common.h"
#include <cmath>
namespace libaa {
class LFO {
public:
    enum class WaveformType { kWaveformSine = 0 };
    LFO() = default;

    float lfo(float phase, WaveformType waveform);

private:
    float lfoSine(float phase) const;
};
} // namespace libaa
