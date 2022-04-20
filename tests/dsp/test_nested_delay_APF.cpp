//
// Created by user on 4/18/22.
//
#include "libaa/dsp/aa_nested_delay_APF.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ANestedDelayAPF : public Test {
public:
    NestedDelayAPF f;
    const float sample_rate = 1;
    const size_t max_delay_size = 128;
    NestedDelayAPF::NestedDelayAPFParameters parameters;
};

TEST_F(ANestedDelayAPF, NeedPrepareBeforeUse) {
    f.prepare(sample_rate, max_delay_size);
}

TEST_F(ANestedDelayAPF, PrepareWillResizeOuterDelaySize) {
    f.prepare(sample_rate, max_delay_size);

    ASSERT_THAT(f.getOuterSimpleDelay().getSize(), Eq(max_delay_size));
}

TEST_F(ANestedDelayAPF, CanUpdateParameters) {
    parameters.inner_apf_delay_ms = 10;
    parameters.inner_apf_g = 0.3;
    parameters.enable_LFO = true;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().inner_apf_delay_ms,
                Eq(parameters.inner_apf_delay_ms));
    ASSERT_THAT(f.getParameters().inner_apf_g, Eq(parameters.inner_apf_g));
    ASSERT_THAT(f.getParameters().enable_LFO, Eq(parameters.enable_LFO));
}

TEST_F(ANestedDelayAPF, UpdateParametersAlsoUpdateDelayParameters) {
    float delay_ms = 10;
    parameters.delay_ms = delay_ms;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getOuterSimpleDelay().getParameters().delay_ms, Eq(delay_ms));
}

TEST_F(ANestedDelayAPF, UpdateParametersAlsoInnerAPFParameters) {
    parameters.inner_apf_delay_ms = 10;
    parameters.inner_apf_g = 0.3;
    parameters.enable_LFO = true;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getInnerAPF().getParameters().apf_g,
                Eq(parameters.inner_apf_g));
    ASSERT_THAT(f.getInnerAPF().getParameters().delay_ms,
                Eq(parameters.inner_apf_delay_ms));
}

TEST_F(ANestedDelayAPF, UpdateParametersAlsoUpdateLPFParameters) {
    float g = 0.5;
    parameters.lpf_g = g;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getOuterLPF().getParameters().lpf_g, Eq(g));
}

TEST_F(ANestedDelayAPF, ProcessGetFilteredSample) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_ms = 1000;
    parameters.inner_apf_g = 0.5;
    parameters.inner_apf_delay_ms = 1000;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(-0.375));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.5625));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.09375));
}

TEST_F(ANestedDelayAPF, ProcessWithLPF) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_ms = 1000;
    parameters.enable_LPF = true;
    parameters.lpf_g = 0.5;
    parameters.inner_apf_g = 0.5;
    parameters.inner_apf_delay_ms = 1000;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.375));
}

TEST_F(ANestedDelayAPF, ProcessWithLFO) {
    f.prepare(sample_rate, max_delay_size);
    parameters.apf_g = 0.5;
    parameters.delay_ms = 1000;
    parameters.enable_LFO = true;
    parameters.lfo_rate_hz = 1;
    parameters.lfo_depth = 1.0;
    parameters.lfo_max_modulation_ms = 0.5;
    parameters.lpf_g = 0.5;
    parameters.inner_apf_g = 0.5;
    parameters.inner_apf_delay_ms = 1000;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(-0.5));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.75));
}