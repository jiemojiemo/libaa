//
// Created by user on 4/17/22.
//
#include "libaa/dsp/aa_delay_APF.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ADelayAPF : public Test {
public:
    DelayAPF f;
    const float sample_rate = 1;
    const size_t max_delay_size = 128;
    DelayAPF::DelayAPFParameters parameters{};
};

TEST_F(ADelayAPF, NeedPrepareBeforeUse) {
    f.prepare(sample_rate, max_delay_size);
}

TEST_F(ADelayAPF, PrepareWillResizeDelayLine) {
    f.prepare(sample_rate, max_delay_size);

    ASSERT_THAT(f.getSimpleDelay().getSize(), Eq(max_delay_size));
}

TEST_F(ADelayAPF, CanUpdateParameters) {
    parameters.apf_g = 0.5;
    parameters.enable_LPF = true;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().apf_g, Eq(parameters.apf_g));
    ASSERT_THAT(f.getParameters().enable_LPF, Eq(parameters.enable_LPF));
}

TEST_F(ADelayAPF, UpdateParametersAlsoUpdateDelayParameters) {
    parameters.delay_params.delay_ms = 10;
    parameters.delay_params.interpolate = true;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getSimpleDelay().getParameters().delay_ms,
                Eq(parameters.delay_params.delay_ms));
    ASSERT_THAT(f.getSimpleDelay().getParameters().interpolate,
                Eq(parameters.delay_params.interpolate));
}

TEST_F(ADelayAPF, UpdateParametersAlsoUpdateLPFParameters) {
    parameters.lpf_params.g = 0.5;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getLPF().getParameters().g, Eq(parameters.lpf_params.g));
}

TEST_F(ADelayAPF, ProcessGetFilteredSample) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_params.delay_ms = 1000;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.75));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.375));
}

TEST_F(ADelayAPF, ProcessWithLPF) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_params.delay_ms = 1000;
    parameters.enable_LPF = true;
    parameters.lpf_params.g = 0.5;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.375));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.1875));
}

TEST_F(ADelayAPF, ProcessWithLFO) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_params.delay_ms = 1000;
    parameters.enable_LFO = true;
    parameters.lfo_rate_hz = 1;
    parameters.lfo_depth = 0.5;
    parameters.lfo_max_modulation_ms = 0.5;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.375));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.46875));
}
