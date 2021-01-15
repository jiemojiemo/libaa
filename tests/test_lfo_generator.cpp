
//
// Created by William.Hua on 2021/1/15.
//

#include "audio_generator/aa_audio_signal_genrator.h"
#include "audio_generator/aa_lfo_generator.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ALFOGenerator : public Test
{
public:
    LFOGenerator lfo;

    const double freq_hz = 20.0;
    const double sr = 44100.0;
};

TEST_F(ALFOGenerator, PhaseIncreasementIsZeroWhenInit)
{
    ASSERT_THAT(lfo.getPhaseIncrement(), DoubleEq(0));
}

TEST_F(ALFOGenerator, PrepareWillRecalculatePhaseInc)
{
    lfo.frequency_hz = freq_hz;
    lfo.prepare(sr);

    ASSERT_THAT(lfo.getPhaseIncrement(), DoubleEq(freq_hz / sr));
}

TEST_F(ALFOGenerator, CanGenerateSineWave)
{
    lfo.frequency_hz = freq_hz;
    lfo.waveform = GeneratorWaveform::kSin;
    lfo.prepare(sr);

    auto output = lfo.renderAudioOutput();

    ASSERT_THAT(output.normal_output, DoubleEq(0.0));
    ASSERT_THAT(output.inverted_output, DoubleEq(-output.normal_output));
    ASSERT_THAT(output.quad_phase_output_pos, DoubleEq(-1.0));
    ASSERT_THAT(output.quad_phase_output_neg, DoubleEq(-output.quad_phase_output_pos));
}

TEST_F(ALFOGenerator, CanGenerateTriangleWave)
{
    lfo.frequency_hz = freq_hz;
    lfo.waveform = GeneratorWaveform::kTriangle;
    lfo.prepare(sr);

    auto output = lfo.renderAudioOutput();

    ASSERT_THAT(output.normal_output, DoubleEq(1.0));
    ASSERT_THAT(output.inverted_output, DoubleEq(-output.normal_output));
    ASSERT_THAT(output.quad_phase_output_pos, DoubleEq(0.0));
    ASSERT_THAT(output.quad_phase_output_neg, DoubleEq(-output.quad_phase_output_pos));
}

TEST_F(ALFOGenerator, CanGenerateSawWave)
{
    lfo.frequency_hz = freq_hz;
    lfo.waveform = GeneratorWaveform::kSaw;
    lfo.prepare(sr);

    auto output = lfo.renderAudioOutput();

    ASSERT_THAT(output.normal_output, DoubleEq(-1.0));
    ASSERT_THAT(output.inverted_output, DoubleEq(-output.normal_output));
    ASSERT_THAT(output.quad_phase_output_pos, DoubleEq(-0.5));
    ASSERT_THAT(output.quad_phase_output_neg, DoubleEq(-output.quad_phase_output_pos));
}