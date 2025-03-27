//
// Created by user on 4/19/22.
//
#include "libaa/processor/aa_processor_utilities.h"
#include "libaa/processor/aa_reverb_tank_processor.h"
#include <array>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;

using namespace libaa;
class AReverbProcessorTank : public Test {
public:
    ReverbTankProcessor proc;
    int num_params = proc.getParameters()->size();
    float sample_rate = 44100;
    const int max_block_size = 1024;
};

TEST_F(AReverbProcessorTank, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("ReverbTank"));
}

TEST_F(AReverbProcessorTank, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(AReverbProcessorTank, ReverbTimeParameterAsExpect) {
    int p_index = 0;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Reverb Time"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.9f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1.0f));
}

TEST_F(AReverbProcessorTank, DampingParameterAsExpect) {
    int p_index = 1;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Damping"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.3f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(0.8f));
}

TEST_F(AReverbProcessorTank, LowShelfFcParameterAsExpect) {
    int p_index = 2;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Low Shelf Fc"));
    ASSERT_THAT(param.getPlainValue(), Eq(150.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(20.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(2000.0f));
}

TEST_F(AReverbProcessorTank, LowShelfGainParameterAsExpect) {
    int p_index = 3;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Low Shelf Gain dB"));
    ASSERT_THAT(param.getPlainValue(), Eq(-20.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-35.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35.0f));
}

TEST_F(AReverbProcessorTank, HighShelfFcParameterAsExpect) {
    int p_index = 4;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("High Shelf Fc"));
    ASSERT_THAT(param.getPlainValue(), Eq(4000.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(1000.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(5000.0f));
}

TEST_F(AReverbProcessorTank, HighShelfGainParameterAsExpect) {
    int p_index = 5;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("High Shelf Gain dB"));
    ASSERT_THAT(param.getPlainValue(), FloatNear(-6.0f, 1e-5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-35.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35.0f));
}

TEST_F(AReverbProcessorTank, DensityParameterAsExpect) {
    int p_index = 6;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Density"));
    ASSERT_THAT(param.getChoiceStrings().size(), Eq(2));
    ASSERT_THAT(param.getChoiceStrings()[0], Eq("Thick"));
    ASSERT_THAT(param.getChoiceStrings()[1], Eq("Thin"));
}

TEST_F(AReverbProcessorTank, PreDelayParameterAsExpect) {
    int p_index = 7;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("PreDelay(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(25.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(100.0f));
}

TEST_F(AReverbProcessorTank, WetDbParameterAsExpect) {
    int p_index = 8;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Wet dB"));
    ASSERT_THAT(param.getPlainValue(), FloatNear(-12.0f, 1e-5));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-60.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20.0f));
}

TEST_F(AReverbProcessorTank, DryDbParameterAsExpect) {
    int p_index = 9;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Dry dB"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-60.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20.0f));
}

TEST_F(AReverbProcessorTank, MaxAPFDelayDbParameterAsExpect) {
    int p_index = 10;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Max APF Delay(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(33.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(100.0f));
}

TEST_F(AReverbProcessorTank, APFDelayWeightDbParameterAsExpect) {
    int p_index = 11;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("APF Delay Weight"));
    ASSERT_THAT(param.getPlainValue(), Eq(0.85f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1.0f));
}

TEST_F(AReverbProcessorTank, MaxFixedDelayDbParameterAsExpect) {
    int p_index = 12;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Max Fixed Delay(ms)"));
    ASSERT_THAT(param.getPlainValue(), Eq(81.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(100.0f));
}

TEST_F(AReverbProcessorTank, DelayWeightDbParameterAsExpect) {
    int p_index = 13;
    auto param = proc.getParameters()->get(p_index);

    ASSERT_THAT(param.getParameterID(), Eq(p_index));
    ASSERT_THAT(param.getParameterName(), Eq("Fixed Delay Weight"));
    ASSERT_THAT(param.getPlainValue(), Eq(1.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(1.0f));
}

TEST_F(AReverbProcessorTank, NeedPrepareBeforeProcess) {
    proc.prepareToPlay(sample_rate, max_block_size);
}

TEST_F(AReverbProcessorTank, CanProcess) {
    AudioBlock block{{{1, 0, 0, 0}, {1, 0, 0, 0}}, num_params};

    sample_rate = 2;
    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    ASSERT_THAT(block.buffer.getWriterPointer(0)[0], FloatNear(0.995172, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(0)[1], FloatNear(0.010529, 1e-5));

    ASSERT_THAT(block.buffer.getWriterPointer(1)[0], FloatNear(1.00483, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(1)[1],
                FloatNear(-0.010529f, 1e-5));
}

TEST_F(AReverbProcessorTank, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(AReverbProcessorTank, SetStateUpdatesParameters) {
    float expected_param_val = 0.1;
    std::string expected_str_val = "Thin";
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Reverb Time"] = expected_param_val;
    state_json["parameters"]["Density"] = expected_str_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float param_val = proc.getParameters()->get(0).getPlainValue();
    std::string param_str_val = proc.getParameters()->get(6).getChoiceString();
    ASSERT_THAT(param_val, Eq(expected_param_val));
    ASSERT_THAT(param_str_val, Eq(expected_str_val));
}