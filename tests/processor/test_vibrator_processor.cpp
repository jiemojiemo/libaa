//
// Created by William on 3/25/22.
//
#include "aa_testing_helper.h"
#include "libaa/processor/aa_vibrator_processor.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class AVibratorProcessor : public Test {
public:
    VibratorProcessor proc;
};

TEST_F(AVibratorProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Vibrator"));
}

TEST_F(AVibratorProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(AVibratorProcessor, RateParameterAsExpect) {
    auto param = proc.getParameters()->get(0);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getParameterName(), Eq("Rate"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.2));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.1));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(20));
}

TEST_F(AVibratorProcessor, DepthParameterAsExpect) {
    auto param = proc.getParameters()->get(1);

    ASSERT_THAT(param.getParameterID(), Eq(1));
    ASSERT_THAT(param.getParameterName(), Eq("Depth"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.5));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(1));
}

TEST_F(AVibratorProcessor, ProcessAsExpect) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, 2};
    float sample_rate = 1;
    int max_block_size = 4;
    float rate_normalized_value =
        proc.getParameters()->get(0).convertPlainValueToNormalizedValue(10);
    float depth = 1.0;
    block.param_changes.push(0, {0, 0, rate_normalized_value});
    block.param_changes.push(1, {1, 0, depth});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0, 0.922973632, 1, 1}, {0, 0.922973632, 1, 1}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}