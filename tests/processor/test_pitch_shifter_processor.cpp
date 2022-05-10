//
// Created by agen on 4/11/22.
//

#include "../aa_testing_helper.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/processor/aa_pitch_shifter_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

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

TEST_F(APitchShifter, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(APitchShifter, SetStateUpdatesParameters) {
    float expected_param_val = 0.5;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Octaves"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float param_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(param_val, Eq(expected_param_val));
}