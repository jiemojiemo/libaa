//
// Created by user on 6/14/22.
//
#include "libaa/processor/aa_time_stretching_processor.h"

#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ATimeStretchingProcessor : public Test {
public:
    TimeStretchingProcessor proc;
    float sample_rate = 16000;
    int max_block_size = 4;
};

TEST_F(ATimeStretchingProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("TimeStretching"));
}

TEST_F(ATimeStretchingProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ATimeStretchingProcessor, HasTimeRatioParameterAsExpect) {
    auto p = proc.getParameters()->get(0);

    ASSERT_THAT(p.getParameterName(), Eq("Time Ratio"));
    ASSERT_THAT(p.getPlainValue(), FloatEq(1.0f));
    ASSERT_THAT(p.getMinPlainValue(), FloatEq(0.1f));
    ASSERT_THAT(p.getMaxPlainValue(), FloatEq(10.0f));
}

TEST_F(ATimeStretchingProcessor, HasPitchScaleParameterAsExpect) {
    auto p = proc.getParameters()->get(1);

    ASSERT_THAT(p.getParameterName(), Eq("Pitch Scale"));
    ASSERT_THAT(p.getPlainValue(), FloatEq(1.0f));
    ASSERT_THAT(p.getMinPlainValue(), FloatEq(0.1f));
    ASSERT_THAT(p.getMaxPlainValue(), FloatEq(8.0f));
}

TEST_F(ATimeStretchingProcessor, PrepareWillBuildInternalTSUtitlites) {
    proc.prepareToPlay(sample_rate, max_block_size);
}

TEST_F(ATimeStretchingProcessor, ProcessUpdateOutputBlockContextNumSamples) {
    AudioBlock input_block{2, 1024, 2};
    AudioBlock output_block(2, 1024, 2);

    float ratio_value = 5;
    float ratio_norm_value = proc.getParameters()->get(0).convertPlainValueToNormalizedValue(ratio_value);
    input_block.param_changes.push(0, {0, 0, ratio_norm_value});
    input_block.context.is_final = false;

    proc.prepareToPlay(sample_rate, 1024);

    for (int i = 0; i < 3; ++i) {
        proc.processBlock(&input_block, &output_block);
    }

    input_block.context.is_final = true;
    proc.processBlock(&input_block, &output_block);

    ASSERT_THAT(output_block.context.num_samples, Eq(7917));
}

TEST_F(ATimeStretchingProcessor, StateAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(ATimeStretchingProcessor, SetStateUpdatesParameters) {
    float expected_param_val = 10.0f;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Time Ratio"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_param_val));
}