//
// Created by user on 4/16/22.
//

#include "libaa/dsp/aa_comb_filter.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ACombFilter : public Test {
public:
    CombFilter f;
    float sample_rate = 1;
    size_t max_delay_size = 2 * sample_rate;
};

TEST_F(ACombFilter, NeedPrepareBeforeUse) {
    f.prepare(sample_rate, max_delay_size);
}

TEST_F(ACombFilter, PrepareWillResizeDelayLine) {
    f.prepare(sample_rate, max_delay_size);

    ASSERT_THAT(f.getSimpleDelay().getSize(), Eq(max_delay_size));
}

TEST_F(ACombFilter, CanUpdateParameters) {
    CombFilter::CombFilterParameters parameters{};
    parameters.rt_60_ms = 10;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().rt_60_ms, Eq(parameters.rt_60_ms));
}

TEST_F(ACombFilter, UpdateRT60ChangeCombFilterFeedback) {
    f.prepare(sample_rate, max_delay_size);
    CombFilter::CombFilterParameters parameters{};
    parameters.rt_60_ms = 200;
    parameters.delay_params.delay_ms = 100;
    ASSERT_THAT(f.getCombFilterFeedback(), Eq(0));

    f.updateParameters(parameters);

    float exponent = -3.0f * parameters.delay_params.delay_ms;
    float expected_value = powf(10.0f, exponent / parameters.rt_60_ms);
    ASSERT_THAT(f.getCombFilterFeedback(), Eq(expected_value));
}

TEST_F(ACombFilter, ProcessGetFilteredSample) {
    f.prepare(sample_rate, max_delay_size);
    CombFilter::CombFilterParameters parameters{};
    parameters.rt_60_ms = 2000;
    parameters.delay_params.delay_ms = 1000;
    parameters.lpf_g = 0.5;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(1.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatNear(0.0316228, 1e-7));
}

TEST_F(ACombFilter, ProcessGetFilteredSampleWithLPF) {
    f.prepare(sample_rate, max_delay_size);
    CombFilter::CombFilterParameters parameters{};
    parameters.rt_60_ms = 1000;
    parameters.delay_params.delay_ms = 1000;
    parameters.lpf_g = 0.8;
    parameters.enable_LPF = true;

    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(1.0));
    ASSERT_THAT(f.processSample(0.0), FloatEq(0.0));
    ASSERT_THAT(f.processSample(0.0), FloatNear(0.001, 1e-7));
}