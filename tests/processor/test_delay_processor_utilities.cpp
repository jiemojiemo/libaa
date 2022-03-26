//
// Created by William on 3/26/22.
//
#include "aa_testing_helper.h"
#include "libaa/processor/aa_delay_processor_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ADelayProcessorUtilities : public Test {
public:
    DelayProcessorUtilities utilities;
    int max_delay_samples = 10;
};

TEST_F(ADelayProcessorUtilities, CanPrepareWithMaxDelaySamples) {
    utilities.prepare(max_delay_samples);
}

TEST_F(ADelayProcessorUtilities, HasTwoDelayLinesAfterPrepare) {
    utilities.prepare(max_delay_samples);

    ASSERT_THAT(utilities.getNumDelayLines(), Eq(2));
}

TEST_F(ADelayProcessorUtilities, DelayLinesSizeIsMaxDelaySamples) {
    utilities.prepare(max_delay_samples);

    ASSERT_THAT(utilities.getNumDelayLines(), Eq(2));
}

TEST_F(ADelayProcessorUtilities, CanProcessWithParameters) {
    AudioBufferX<float> in_out{{{1, 1, 1, 1}, {2, 2, 2, 2}}};
    float delay_samples = 2;
    float feedback = 1;
    float dry = 0;
    float wet = 1;

    utilities.prepare(max_delay_samples);
    utilities.process(&in_out, delay_samples, feedback, dry, wet);

    AudioBufferX<float> expected{{{0, 0, 0, 1}, {0, 0, 0, 2}}};

    ASSERT_THAT(in_out, Eq(expected));
}