
//
// Created by William.Hua on 2021/8/3.
//

#include <gmock/gmock.h>
#include "aa_test_helper.h"
#include "libaa/core/aa_audio_block.h"
#include <list>
#include <vector>

using namespace testing;
using namespace libaa;

class AAudioBlock : public Test
{
public:
};

TEST_F(AAudioBlock, ContructWithEmptyAudioBuffer)
{
    AudioBlock audio_block;

    ASSERT_THAT(audio_block.audio_buffer.getNumberFrames(), Eq(0));
}

TEST_F(AAudioBlock, CanConstructWithListOfSamples)
{
    AudioBlock audio_block {{
                                {1.0f, 1.0f},
                                {2.0f, 2.0f}
    }};

    ASSERT_THAT(audio_block.audio_buffer.getNumberFrames(), Eq(2));
    ASSERT_THAT(audio_block.audio_buffer.getNumberChannels(), Eq(2));
}

TEST_F(AAudioBlock, CanConstructWithAudioBufferSize)
{
    int num_chan = 2;
    int num_frames = 3;
    AudioBlock audio_block(num_chan, num_frames);

    ASSERT_THAT(audio_block.audio_buffer.getNumberFrames(), Eq(num_frames));
    ASSERT_THAT(audio_block.audio_buffer.getNumberChannels(), Eq(num_chan));
}

TEST_F(AAudioBlock, ConstructWithEmptyParameterChanges)
{
    AudioBlock audio_block;

    ASSERT_THAT(audio_block.param_changes.getParameterChangesArray().size(), Eq(0));
}

TEST_F(AAudioBlock, CanMoveConstructWithAudioBufferAndParameterChanges)
{
    auto audio_buffer = AudioBufferNew<float>{{
                                                  {1.0f, 1.0f},
                                                  {2.0f, 2.0f}
                                              }};
    auto param_changes = ParameterChanges{5};

    auto audio_block = AudioBlock{ audio_buffer, param_changes };

    ASSERT_THAT(audio_block.audio_buffer, Eq(audio_buffer));
    ASSERT_THAT(audio_block.param_changes.getParameterChangesArray().size(),
                Eq(param_changes.getParameterChangesArray().size()));
}