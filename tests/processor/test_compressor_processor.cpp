//
// Created by user on 5/19/22.
//
#include "libaa/processor/aa_compressor_processor.h"

#include "../aa_testing_helper.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
using namespace testing;
using namespace libaa;
class ACompressor : public Test {
public:
    Compressor proc;
};

TEST_F(ACompressor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Compressor"));
}

TEST_F(ACompressor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ACompressor, ThresholdParameterAsExpected) {
    int param_index = 0;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getParameterName(), Eq("Threshold"));
    ASSERT_THAT(param.getPlainValue(), Eq(-10));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-40));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(0));
}

TEST_F(ACompressor, KneeWidthAsExpected) {
    int param_index = 1;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Knee Width"));
    ASSERT_THAT(param.getPlainValue(), Eq(5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20));
}

TEST_F(ACompressor, RatioParameterAsExpected) {
    int param_index = 2;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Ratio"));
    ASSERT_THAT(param.getPlainValue(), Eq(1));
    ASSERT_THAT(param.getMinPlainValue(), Eq(1));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20));
}

TEST_F(ACompressor, AttackTimeParameterAsExpected) {
    int param_index = 3;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Attack Time(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(1));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(100));
}

TEST_F(ACompressor, ReleaseTimeParameterAsExpected) {
    int param_index = 4;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Release Time(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(500));
    ASSERT_THAT(param.getMinPlainValue(), Eq(1));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(5000));
}

TEST_F(ACompressor, OutputGainParameterAsExpected) {
    int param_index = 5;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Output Gain dB"));
    ASSERT_THAT(param.getPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-20));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20));
}

TEST_F(ACompressor, SoftKneeParameterAsExpected) {
    int param_index = 6;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Soft Knee"));
    ASSERT_THAT(param.getParameterType(), Eq(ParameterType::kBool));
    ASSERT_THAT(param.getBool(), Eq(true));
}

TEST_F(ACompressor, PrepareWillUpdateUtilitiesParameters) {
    float sample_rate = 1;
    int max_block_size = 4;
    proc.prepareToPlay(sample_rate, max_block_size);

    ASSERT_THAT(proc.getUtilities()[0].detector.getSampleRate(), Eq(sample_rate));
    ASSERT_THAT(proc.getUtilities()[0].getParameters().threshold_db, Eq(proc.getParameters()->get(0).getPlainValue()));
}

TEST_F(ACompressor, ProcessAsExpect) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, (int)(proc.getParameters()->size())};
    float sample_rate = 1;
    int max_block_size = 4;

    // set ratio to 20
    block.param_changes.push(2, {2, 0, 1.0});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0.334965438, 0.334965408, 0.334965408, 0.334965408}, {0.334965438, 0.334965408, 0.334965408, 0.334965408}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}

TEST_F(ACompressor, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(ACompressor, SetStateUpdatesParameters) {
    float expected_param_val = -5;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Threshold"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_param_val));
}