//
// Created by agen on 4/11/22.
//

#include "../aa_testing_helper.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_pitch_shifter_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class APitchShifter : public Test {
public:
    PitchShiftProcessor proc;

    int sample_rate = 44100;
    int max_block_size = 1024;
};

TEST_F(APitchShifter, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("PitchShifter"));
}

TEST_F(APitchShifter, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(APitchShifter, OctavesParamterAsExpected) {
    int p_index = 0;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Octaves"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-2.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(2.0f));
}

TEST_F(APitchShifter, SemitonesParamterAsExpected) {
    int p_index = 1;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Semitones"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-12.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(12.0f));
}

TEST_F(APitchShifter, CentsParamterAsExpected) {
    int p_index = 2;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Cents"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.0f));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-100.0f));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(100.0f));
}

TEST_F(APitchShifter, HasNullImplWhenInit) {
    ASSERT_THAT(proc.getPitchShifterImpl(), IsNull());
}

TEST_F(APitchShifter, HasNotNullImplAfterPrepare) {
    proc.prepareToPlay(sample_rate, max_block_size);

    ASSERT_THAT(proc.getPitchShifterImpl(), NotNull());
}

TEST_F(APitchShifter, HasLatency) {
    proc.prepareToPlay(sample_rate, max_block_size);

    ASSERT_THAT(proc.getLatencySamples(), Eq(8192));
}

TEST_F(APitchShifter, ReturnsNeg1LatencyBeforePrepare) {
    ASSERT_THAT(proc.getLatencySamples(), Eq(-1));
}

TEST_F(APitchShifter, ProcessAsExpected) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, 2};
    block.param_changes.push(0, {0, 0, 0.5});
    block.param_changes.push(1, {1, 0, 0.5});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0, 0, 0, 0}, {0, 0, 0, 0}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}