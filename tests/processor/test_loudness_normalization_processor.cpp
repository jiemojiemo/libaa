//
// Created by user on 11/29/22.
//
#include <gmock/gmock.h>

#include "../aa_testing_helper.h"
#include "libaa/processor/aa_loudness_normalization_processor.h"
#include "libaa/processor/aa_processor_utilities.h"

using namespace testing;
using namespace libaa;
class ALoudnessNormalizationProcessor : public Test {
public:
    LoudnessNormalizationProcessor proc;
};

TEST_F(ALoudnessNormalizationProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("LoudnessNormalization"));
}

TEST_F(ALoudnessNormalizationProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(ALoudnessNormalizationProcessor, TargetLoudnessParameterAsExpected) {
    int param_index = 0;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Target Loudness LUFS"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(-16));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-70));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(0));
}

TEST_F(ALoudnessNormalizationProcessor, TargetTruePeakParameterAsExpected) {
    int param_index = 1;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Target True Peak dB"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(-1));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-9));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(0));
}

TEST_F(ALoudnessNormalizationProcessor, AutoGainParameterAsExpected) {
    int param_index = 2;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterType(), Eq(ParameterType::kBool));
    ASSERT_THAT(param.getParameterName(), Eq("Live Mode"));
    ASSERT_THAT(param.getBool(), Eq(true));
}

TEST_F(ALoudnessNormalizationProcessor, MeasuredLoudnessParameterAsExpected) {
    int param_index = 3;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Measured Loudness LUFS"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-99));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(0));
}

TEST_F(ALoudnessNormalizationProcessor, MeasuredTruePeakParameterAsExpected) {
    int param_index = 4;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Measured True Peak dB"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-99));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(0));
}

TEST_F(ALoudnessNormalizationProcessor, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}
TEST_F(ALoudnessNormalizationProcessor, SetStateUpdatesParameters) {
    float expected_param_val = -8;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Target Loudness LUFS"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, FloatEq(expected_param_val));
}

TEST_F(ALoudnessNormalizationProcessor, ProcessLiveModeAsExpected) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, (int)(proc.getParameters()->size())};
    float sample_rate = 44100;
    int max_block_size = 4;

    block.param_changes.push(1, {1, 0, 1.0});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0.865964353, 0.894770503, 0.919055759, 0.939300715}, {0.865964353, 0.894770503, 0.919055759, 0.939300715}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}

TEST_F(ALoudnessNormalizationProcessor, ProcessOfflineModeAsExpected) {
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}, (int)(proc.getParameters()->size())};
    float sample_rate = 44100;
    int max_block_size = 4;

    block.param_changes.push(2, {2, 0, 0.0});

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0.812830507, 0.64183253, 0.470834434, 0.299833447}, {0.812830507, 0.64183253, 0.470834434, 0.299833447}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}