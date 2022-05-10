//
// Created by William on 3/23/22.
//
#include "libaa/aa_version.h"
#include "libaa/dsp/aa_db_utils.h"
#include "libaa/processor/aa_gain_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <nlohmann/json.hpp>

#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AGainProcessor : public Test {
public:
    GainProcessor proc;
    float sr = 44100;
    int max_block_size = 4;
};

TEST_F(AGainProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Gain"));
}

TEST_F(AGainProcessor, RetunrsGainDbParameterAsExpect) {
    auto param = proc.getParameters()->get(0);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-96.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35.0f));
}

TEST_F(AGainProcessor, CanInitWithDb) {
    GainProcessor proc(20);

    auto param = proc.getParameters()->get(0);

    ASSERT_THAT(param.getParameterID(), Eq(0));
    ASSERT_THAT(param.getPlainValue(), Eq(20.0f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-96.0f));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35.0f));
}

TEST_F(AGainProcessor, HasNoLantency) {
    ASSERT_THAT(proc.getLatencySamples(), Eq(0));
}

TEST_F(AGainProcessor, HasZeroTailLengthSeconds) {
    ASSERT_THAT(proc.getTailLengthSeconds(), Eq(0));
}

TEST_F(AGainProcessor, ProcessChangeInputDb) {
    float boost_db = -20;
    float boost_scale = dbToScale(boost_db);
    GainProcessor proc(boost_db);

    AudioBlock block{
        {{1, 1, 1, 1}, {2, 2, 2, 2}},
    };
    float sr = 44100;
    int max_block_size = 4;
    proc.prepareToPlay(sr, max_block_size);
    proc.processBlock(&block, &block);

    ASSERT_THAT(block.buffer.getReadPointer(0)[0], Eq(1 * boost_scale));
    ASSERT_THAT(block.buffer.getReadPointer(1)[0], Eq(2 * boost_scale));
}

TEST_F(AGainProcessor, ProcessChangeInputDbWithParameterChange) {
    float boost_db = -20;
    float boost_scale = dbToScale(boost_db);
    float normalized_db =
        proc.getParameters()->get(0).convertPlainValueToNormalizedValue(
            boost_db);

    AudioBlock block{{{1, 1, 1, 1}, {2, 2, 2, 2}}, 1};
    block.param_changes.push(0, {0, 0, normalized_db});

    proc.prepareToPlay(sr, max_block_size);
    proc.processBlock(&block, &block);

    ASSERT_THAT(block.buffer.getReadPointer(0)[0], Eq(1 * boost_scale));
    ASSERT_THAT(block.buffer.getReadPointer(1)[0], Eq(2 * boost_scale));
}

TEST_F(AGainProcessor, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(AGainProcessor, SetStateUpdatesParameters) {
    float expected_gain_val = 10;
    auto state_str = ProcessorUtilities::serializeProcessorToString(&proc);
    auto state_json = nlohmann::json::parse(state_str);
    state_json["parameters"]["Gain dB"] = expected_gain_val;
    auto new_state_str = state_json.dump();

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float gain_val = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(gain_val, Eq(expected_gain_val));
}