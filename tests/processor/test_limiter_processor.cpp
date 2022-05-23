//
// Created by user on 5/23/22.
//
#include "libaa/processor/aa_limiter_processor.h"

#include "../aa_testing_helper.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;
class ALimiter : public Test {
public:
    LimiterProcessor proc;
};

TEST_F(ALimiter, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Limiter"));
}

TEST_F(ALimiter, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ALimiter, ThresholdParameterAsExpected) {
    int param_index = 0;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Threshold"));
    ASSERT_THAT(param.getPlainValue(), Eq(-10));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-40));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(0));
}

TEST_F(ALimiter, InputGainParameterAsExpected) {
    int param_index = 1;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Input Gain dB"));
    ASSERT_THAT(param.getPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35));
}

TEST_F(ALimiter, ReleaseTimeParameterAsExpected) {
    int param_index = 2;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Release Time(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(500));
    ASSERT_THAT(param.getMinPlainValue(), Eq(1));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(5000));
}

TEST_F(ALimiter, PrepareWillUpdateUtilitiesParameters) {
    float sample_rate = 1;
    int max_block_size = 4;
    proc.prepareToPlay(sample_rate, max_block_size);

    ASSERT_THAT(proc.getUtilities()[0].detector.getSampleRate(), Eq(sample_rate));
    ASSERT_THAT(proc.getUtilities()[0].getParameters().threshold_db, Eq(proc.getParameters()->get(0).getPlainValue()));
}

TEST_F(ALimiter, ProcessAsExpect) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, (int)(proc.getParameters()->size())};
    float sample_rate = 1;
    int max_block_size = 4;

    // set ratio to 35
    block.param_changes.push(1, {1, 0, 1.0});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0.316227764, 0.316227764, 0.316227764, 0.316227764}, {0.316227764, 0.316227764, 0.316227764, 0.316227764}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}

TEST_F(ALimiter, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(ALimiter, SetStateUpdatesParameters) {
    float expected_param_val = -5;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Threshold"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_param_val));
}
