
//
// Created by William.Hua on 2021/1/15.
//

#pragma once

namespace libaa
{

enum class GeneratorWaveform{
    kTriangle = 0,
    kSin,
    kSaw
};

struct SignalGenData
{
    double normal_output            = 0.0;
    double inverted_output          = 0.0;
    double quad_phase_output_pos    = 0.0;
    double quad_phase_output_neg    = 0.0;
};

class AudioSignalGenerator
{
public:
    virtual void prepare(double sample_rate) = 0;

    virtual SignalGenData renderAudioOutput() = 0;
};
}
