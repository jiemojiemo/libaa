//
// Created by William on 3/24/22.
//
#include "aa_testing_helper.h"
#include "libaa/processor/aa_delay_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ADelayProcessor : public Test {
public:
    DelayProcessor proc;
};

TEST_F(ADelayProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Delay"));
}

TEST_F(ADelayProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ADelayProcessor, DelayTimeParameterAsExpect) {
    auto param = proc.getParameters()->get(0);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getParameterName(), Eq("Delay Time(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(2000));
}

TEST_F(ADelayProcessor, FeedbackParameterAsExpect) {
    auto param = proc.getParameters()->get(1);

    ASSERT_THAT(param.getParameterID(), Eq(1));
    ASSERT_THAT(param.getParameterName(), Eq("Feedback"));
    ASSERT_THAT(param.getPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1));
}

TEST_F(ADelayProcessor, DryParameterAsExpect) {
    auto param = proc.getParameters()->get(2);

    ASSERT_THAT(param.getParameterID(), Eq(2));
    ASSERT_THAT(param.getParameterName(), Eq("Dry"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1));
}

TEST_F(ADelayProcessor, WetParameterAsExpect) {
    auto param = proc.getParameters()->get(3);

    ASSERT_THAT(param.getParameterID(), Eq(3));
    ASSERT_THAT(param.getParameterName(), Eq("Wet"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1));
}

TEST_F(ADelayProcessor, HasNoLantency) {
    ASSERT_THAT(proc.getLatencySamples(), Eq(0));
}

TEST_F(ADelayProcessor, HasZeroTailLengthSeconds) {
    ASSERT_THAT(proc.getTailLengthSeconds(), Eq(0));
}

TEST_F(ADelayProcessor, ProcessAsExpect) {
    AudioBlock block{{{1, 1, 1, 1}, {2, 2, 2, 2}}, 4};

    int sample_rate = 10;
    int max_block_size = 4;
    int delay_samples = 2;
    float max_delay = proc.getParameters()->get(0).getMaxPlainValue();
    float delay_ms_normalized_value =
        float(delay_samples) / sample_rate / max_delay * 1000.0;
    float feedback = 1;
    float dry = 0;
    float wet = 1;
    block.param_changes.push(0, {0, 0, delay_ms_normalized_value});
    block.param_changes.push(1, {1, 0, feedback});
    block.param_changes.push(2, {2, 0, dry});
    block.param_changes.push(3, {3, 0, wet});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0, 0, 0, 1}, {0, 0, 0, 2}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}
