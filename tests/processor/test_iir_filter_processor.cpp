//
// Created by user on 5/31/22.
//
#include "../aa_testing_helper.h"
#include "libaa/processor/aa_iir_filter_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;
class AFilterProcessor : public Test {
public:
    IIRFilterProcessor proc;
};

TEST_F(AFilterProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("IIRFilter"));
}

TEST_F(AFilterProcessor, HasParameters) {
    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(AFilterProcessor, TypeParameterAsExpected) {
    int param_index = 0;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterType(), Eq(ParameterType::kChoice));
    ASSERT_THAT(param.getParameterName(), Eq("Type"));
    ASSERT_THAT(param.getChoiceStrings(), ElementsAre("LPF1",
                                                      "HPF1",
                                                      "LPF2",
                                                      "HPF2",
                                                      "BPF2",
                                                      "BSF2",
                                                      "APF1",
                                                      "APF2",
                                                      "LowShelf",
                                                      "HighShelf",
                                                      "NCQParaEQ",
                                                      "CQParaEQ"));
}

TEST_F(AFilterProcessor, CutoffParameterAsExpected) {
    int param_index = 1;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Cutoff"));
    ASSERT_THAT(param.getPlainValue(), Eq(1000));
    ASSERT_THAT(param.getMinPlainValue(), Eq(20));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20000));
}

TEST_F(AFilterProcessor, QParameterAsExpected) {
    int param_index = 2;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Q"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0.707));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(0.707));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20));
}

TEST_F(AFilterProcessor, BoostCutParameterAsExpecte) {
    int param_index = 3;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Boost/Cut"));
    ASSERT_THAT(param.getPlainValue(), FloatEq(0));
    ASSERT_THAT(param.getMinPlainValue(), FloatEq(-48));
    ASSERT_THAT(param.getMaxPlainValue(), FloatEq(48));
}

TEST_F(AFilterProcessor, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(AFilterProcessor, SetStateUpdatesParameters) {
    float expected_param_val = 10;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Q"] = expected_param_val;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(2).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_param_val));
}

TEST_F(AFilterProcessor, ProcessAsExpect) {
    int sample_rate = 4;
    int max_block_size = 4;
    AudioBlock block{{{1, 1, 1, 1}, {1, 1, 1, 1}}};

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBlock expected{{{0.900575518, 1.07965398, 0.936185061, 1.05112529}, {0.900575518, 1.07965398, 0.936185061, 1.05112529}}};

    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}
