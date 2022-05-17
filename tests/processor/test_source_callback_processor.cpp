//
// Created by user on 5/9/22.
//
#include "libaa/processor/aa_processor_utilities.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ASourceCallbackProcessor : public Test {
public:
    SourceCallbackProcessor proc;
    float sample_rate = 44100;
    int max_block_size = 128;
};

TEST_F(ASourceCallbackProcessor, CanReportCorrectName) {
    ASSERT_THAT(proc.getName(), Eq("Source Callback"));
}

TEST_F(ASourceCallbackProcessor, HasNoParameters) {
    ASSERT_THAT(proc.getParameters(), IsNull());
}

TEST_F(ASourceCallbackProcessor, CanSetSourceCallbackFunction) {
    auto callback = [](AudioBlock *output_block) {};
    proc.setSourceCallback(callback);

    ASSERT_THAT(proc.getSourceCallback(), NotNull());
}

TEST_F(ASourceCallbackProcessor, SourceCallbackToGenenrateDataWhenProcess) {
    AudioBlock block{2, 3, 0};
    AudioBufferX<float> data{
        {{1, 1, 1}, {2, 2, 2}}};
    auto callback = [&data](AudioBlock *output_block) {
        ASSERT_THAT(output_block->buffer.getNumberChannels(), Eq(2));
        ASSERT_THAT(output_block->buffer.getNumberFrames(), Eq(3));
        output_block->buffer.copyFrom(&data, 2, 3, 0, 0);
    };

    proc.setSourceCallback(callback);
    proc.prepareToPlay(44100, 128);
    proc.processBlock(&block, &block);

    ASSERT_THAT(block.buffer.getReadPointer(0)[0], Eq(1));
    ASSERT_THAT(block.buffer.getReadPointer(1)[0], Eq(2));
}

TEST_F(ASourceCallbackProcessor, PrcessoDoNothingIfCallbackIsNotSet) {
    AudioBlock block{
        {{1, 2, 3}, {4, 5, 6}}};

    proc.prepareToPlay(44100, 128);
    proc.processBlock(&block, &block);

    ASSERT_THAT(proc.getSourceCallback(), IsNull());
    ASSERT_THAT(block.buffer.getReadPointer(0)[0], Eq(1));
    ASSERT_THAT(block.buffer.getReadPointer(1)[0], Eq(4));
}

TEST_F(ASourceCallbackProcessor, CanPushParameterChange) {
    int num_params = 10;
    int param_index = 0;
    float norm_val = 0.5;
    AudioBlock block{0, 0, num_params};
    proc.prepareToPlay(44100, 128);
    proc.pushParameterChange(param_index, norm_val);

    proc.processBlock(&block, &block);

    ParameterChangePoint result;
    bool ok = block.param_changes.pop(0, result);
    ASSERT_TRUE(ok);
    ASSERT_THAT(result.index, Eq(param_index));
    ASSERT_THAT(result.normalized_value, Eq(norm_val));
}

TEST_F(ASourceCallbackProcessor, StateStringAsExpected) {
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}

TEST_F(ASourceCallbackProcessor, SetStateDonothing) {
    auto state = proc.getState();

    proc.setState(state.data(), state.size());
}