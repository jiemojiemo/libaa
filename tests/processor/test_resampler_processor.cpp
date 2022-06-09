//
// Created by user on 5/17/22.
//
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"
#include "libaa/processor/aa_resampler_processor.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;
class AResamplerProcessor : public Test {
public:
    ResamplerProcessor proc;
    float sample_rate = 44100;
    int max_block_size = 16;
};

TEST_F(AResamplerProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Resampler"));
}

TEST_F(AResamplerProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(AResamplerProcessor, RatioParameterAsExpected) {
    auto param = proc.getParameters()->get(0);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getParameterName(), Eq("Ratio"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(1.0));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.1));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(10));
}

TEST_F(AResamplerProcessor, PrepareWillBuildInternalResampler) {
    proc.prepareToPlay(sample_rate, max_block_size);
}

TEST_F(AResamplerProcessor, ProcessUpdateOutputBlockContextNumSamples) {
    AudioBlock input_block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, 1};
    AudioBlock output_block(2, 4, 1);

    float ratio_value = 5;
    float ratio_norm_value = proc.getParameters()->get(0).convertPlainValueToNormalizedValue(ratio_value);
    input_block.param_changes.push(0, {0, 0, ratio_norm_value});
    input_block.context.is_final = true;

    proc.prepareToPlay(sample_rate, 4);
    proc.processBlock(&input_block, &output_block);

    ASSERT_THAT(output_block.context.num_samples, Eq(ratio_value * input_block.context.num_samples));
}

TEST_F(AResamplerProcessor, StateAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(AResamplerProcessor, SetStateUpdatesParameters) {
    float expected_param_val = 10.0f;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Ratio"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_param_val));
}