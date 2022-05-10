//
// Created by user on 5/8/22.
//
#include "libaa/processor/aa_source_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ASourceProcessor : public Test {
public:
    float sample_rate = 44100;
    float max_block_size = 10;
};

TEST_F(ASourceProcessor, CanReportCorrectName) {
    SourceProcessor proc;

    ASSERT_THAT(proc.getName(), Eq("Source Processor"));
}

TEST_F(ASourceProcessor, HasNoParameters) {
    SourceProcessor proc;

    ASSERT_THAT(proc.getParameters(), IsNull());
}

TEST_F(ASourceProcessor, CanInitWithChannelData) {
    SourceProcessor proc{
        {{1, 2, 3},
         {4, 5, 6}}};

    ASSERT_THAT(proc.getChannelData()->getNumberChannels(), Eq(2));
    ASSERT_THAT(proc.getChannelData()->getNumberFrames(), Eq(3));
}

TEST_F(ASourceProcessor, ProcessWillCopyChannelDataToOutput) {
    SourceProcessor proc{
        {{1, 2, 3},
         {4, 5, 6}}};

    AudioBlock block{2, 3, 0};

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    ASSERT_THAT(block.buffer.getReadPointer(0)[0], Eq(1));
    ASSERT_THAT(block.buffer.getReadPointer(1)[0], Eq(4));
}


TEST_F(ASourceProcessor, StateStringAsExpected) {
    SourceProcessor proc{
        {{1, 2, 3},
         {4, 5, 6}}};
    auto state = proc.getState();

    auto expected_string = ProcessorUtilities::serializeProcessorToString(&proc);

    ASSERT_THAT(ProcessorUtilities::convertProcessorStateToString(state), Eq(expected_string));
}