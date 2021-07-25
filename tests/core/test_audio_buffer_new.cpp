
//
// Created by William.Hua on 2021/7/25.
//

#include <gmock/gmock.h>
#include "libaa/core/aa_audio_buffer_new.h"
using namespace testing;
using namespace std;
using namespace libaa;

class AAudioBufferNew : public Test
{
public:
    void SetUp() override
    {

    }

    int num_channels = 2;
    int num_frames = 2;
    list<vector<float>> channel_data = {{1,2,3}, {4, 5, 6}};
};

TEST_F(AAudioBufferNew, CanContructWithSize)
{
    AudioBufferNew<float> audio_buffer(num_channels, num_frames);
}

TEST_F(AAudioBufferNew, ConstructWithSizeWillFillZerosToBuffer)
{
    AudioBufferNew<float> audio_buffer(num_channels, num_frames);

    for(int c = 0; c < num_channels; ++c)
    {
        for(int i = 0; i < num_frames; ++i)
        {
            ASSERT_THAT(audio_buffer.getReadPointer(c)[i], Eq(0.0f));
        }
    }
}

TEST_F(AAudioBufferNew, CanGetNumberOfChannels)
{
    AudioBufferNew<float> audio_buffer(num_channels, num_frames);
    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(num_channels));
}

TEST_F(AAudioBufferNew, CanGetNumberOfFrames)
{
    AudioBufferNew<float> audio_buffer(num_channels, num_frames);
    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(num_channels));
}

TEST_F(AAudioBufferNew, CanConstructWithInitialzerList)
{
    AudioBufferNew<float> audio_buffer(channel_data);
}

TEST_F(AAudioBufferNew, ConstructWithListCanGetNumChannels)
{
    AudioBufferNew<float> audio_buffer(channel_data);

    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(2));
}

TEST_F(AAudioBufferNew, CanGetNumberOfFramesEqualsTheMaxFramesOfAllChannel)
{
    list<vector<float>> channel_data_with_diff_size = {{1,2,3}, {2}};

    AudioBufferNew<float> audio_buffer(channel_data_with_diff_size);

    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(3));
}

TEST_F(AAudioBufferNew, DoCopyWhenConstructWithList)
{
    list<vector<float>> channel_data_with_diff_size = {{1,2,3}, {2}};

    AudioBufferNew<float> audio_buffer(channel_data_with_diff_size);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], Eq(1.0));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[1], Eq(2.0));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[2], Eq(3.0));

    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], Eq(2));
}