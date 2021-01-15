
//
// Created by William.Hua on 2021/1/15.
//

#pragma once
#include "audio_generator/aa_audio_signal_genrator.h"
#include <cmath>
#include <memory>
namespace libaa
{
class LFOGenerator : public AudioSignalGenerator
{
public:
    LFOGenerator();

    void prepare(double sample_rate) override;

    SignalGenData renderAudioOutput() override;

    double getPhaseIncrement() const;

    double frequency_hz        = {0.0f};
    GeneratorWaveform waveform = {GeneratorWaveform::kTriangle};
private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
