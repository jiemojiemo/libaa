
//
// Created by William.Hua on 2021/8/3.
//

#include "aa_testing_helper.h"
#include "libaa/core/aa_audio_block.h"
#include <gmock/gmock.h>
#include <list>
#include <vector>

using namespace testing;
using namespace libaa;

class AAudioBlock : public Test {
public:
};

TEST_F(AAudioBlock, DefalutConstructHasEmptryAudioBuffer) {
    AudioBlock block;

    ASSERT_THAT(block.buffer.getNumberChannels(), Eq(0));
    ASSERT_THAT(block.buffer.getNumberFrames(), Eq(0));
}

TEST_F(AAudioBlock, DefalutConstructHasEmptryParameterChanges) {
    AudioBlock block;

    ASSERT_THAT(block.param_changes.getNumParameters(), Eq(0));
}

TEST_F(AAudioBlock, CanConstructWithAudioBufferInitList) {
    AudioBlock block{
        {{1, 2, 3}, {4, 5, 6}},
    };

    ASSERT_THAT(block.buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(block.buffer.getNumberFrames(), Eq(3));
}

TEST_F(AAudioBlock, CanConstructWithAudioBufferInitListAndNumParams) {
    AudioBlock block{{{1, 2, 3}, {4, 5, 6}}, 10};

    ASSERT_THAT(block.buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(block.buffer.getNumberFrames(), Eq(3));
    ASSERT_THAT(block.param_changes.getNumParameters(), Eq(10));
}

TEST_F(AAudioBlock, CanConstructWithChannelsFramesAndNumParameters) {
    AudioBlock block{2, 3, 10};

    ASSERT_THAT(block.buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(block.buffer.getNumberFrames(), Eq(3));
    ASSERT_THAT(block.param_changes.getNumParameters(), Eq(10));
}

TEST_F(AAudioBlock, DefaultProcessingConfigHasZeroNumSamples) {
    AudioBlock block;

    ASSERT_THAT(block.context.num_samples, Eq(0));
}

TEST_F(AAudioBlock, InitWithAudioBufferResetContextNumSamples) {
    AudioBlock block{
        {{1, 2, 3}, {4, 5, 6}},
    };

    ASSERT_THAT(block.context.num_samples, Eq(3));
}

TEST_F(AAudioBlock, InitWithChannelsAndFramesResetContextNumSamples) {
    int num_samples = 5;
    AudioBlock block{2, num_samples, 10};

    ASSERT_THAT(block.context.num_samples, Eq(num_samples));
}

TEST_F(AAudioBlock, ResizeChangeAudioBufferChannelsAndFrames) {
    int new_frames = 10;
    AudioBlock block(2, 0, 0);

    block.resizeFrames(new_frames);

    ASSERT_THAT(block.buffer.getNumberFrames(), Eq(new_frames));
}

TEST_F(AAudioBlock, ResizeChangeProcessingContextNumSamples) {
    int new_frames = 10;
    AudioBlock block(2, 0, 0);

    block.resizeFrames(new_frames);

    ASSERT_THAT(block.context.num_samples, Eq(new_frames));
}
