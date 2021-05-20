//
// Created by william on 2020/2/26.
//
#include "libaa/dsp/aa_lfo.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;


TEST(ALFO, CalcValueWithPhaseAndWaveType)
{
    LFO lfo;
    float phase = 0.0;

    phase = 1.0;
    EXPECT_THAT(lfo.lfo(phase, LFO::WaveformType::kWaveformSine), FloatEq(0.5 + 0.5*sin(2*M_PI*phase)));
}