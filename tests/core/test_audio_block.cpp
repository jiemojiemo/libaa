
//
// Created by William.Hua on 2021/8/3.
//

#include "aa_test_helper.h"
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
    AudioBlock audio_block;

    ASSERT_THAT(audio_block.buffer.getNumberChannels(), Eq(0));
    ASSERT_THAT(audio_block.buffer.getNumberFrames(), Eq(0));
}

TEST_F(AAudioBlock, DefalutConstructHasEmptryParameterChanges) {
    AudioBlock audio_block;

    ASSERT_THAT(audio_block.param_changes.getNumParameters(), Eq(0));
}

TEST_F(AAudioBlock, CanConstructWithAudioBufferInitList) {
    AudioBlock audio_block{
        {{1, 2, 3}, {4, 5, 6}},
    };

    ASSERT_THAT(audio_block.buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(audio_block.buffer.getNumberFrames(), Eq(3));
}

TEST_F(AAudioBlock, CanConstructWithAudioBufferInitListAndNumParams) {
    AudioBlock audio_block{{{1, 2, 3}, {4, 5, 6}}, 10};

    ASSERT_THAT(audio_block.buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(audio_block.buffer.getNumberFrames(), Eq(3));
    ASSERT_THAT(audio_block.param_changes.getNumParameters(), Eq(10));
}