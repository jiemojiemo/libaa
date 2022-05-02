//
// Created by user on 5/2/22.
//
#include "libaa/processor/aa_schroeder_reverb_processor.h"
#include <array>
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ASchroederProcessor : public Test {
public:
    SchroederReverbProcessor proc;
};

TEST_F(ASchroederProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("SchroederReverb"));
}

TEST_F(ASchroederProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ASchroederProcessor, ReverbTimeParameterAsExpect) {
    int p_index = 0;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Reverb Time(ms)"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(50.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(2000.0f));
}

TEST_F(ASchroederProcessor, WetParameterAsExpect) {
    int p_index = 1;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Wet"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(1.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(1.0f));
}

TEST_F(ASchroederProcessor, DryParameterAsExpect) {
    int p_index = 2;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Dry"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(1.0f));
}

TEST_F(ASchroederProcessor, CanProcess) {
    int num_params = proc.getParameters()->size();
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, num_params};

    float sample_rate = 2;
    int max_block_size = 4;
    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    EXPECT_THAT(block.buffer.getWriterPointer(0)[0], FloatEq(0.0f));
    EXPECT_THAT(block.buffer.getWriterPointer(1)[0], FloatEq(0.0f));
}