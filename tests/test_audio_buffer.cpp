//
// Created by william on 2020/2/22.
//
#include "audio_basics/aa_audio_buffer.h"

#include <gmock/gmock.h>

using namespace libaa;
using namespace testing;

class AAudioBuffer : public Test
{
public:
    void SetUp() override
    {
        for(int i = 0; i < num_samples; ++i)
        {
            left_data[i] = static_cast<float>(i);
            right_data[i] = left_data[i];
        }

        external_data.emplace_back(left_data);
        external_data.emplace_back(right_data);
    }
    constexpr static int num_samples = 512;
    constexpr static int num_channels = 2;
    float left_data[num_samples];
    float right_data[num_samples];

    vector<float*> external_data;
    int num_channel_to_use = 2;
    int start_sample = 0;

};

TEST_F(AAudioBuffer, CreatAnEmptyBuffer)
{
    AudioBuffer<float> empty_buffer;

    int samples = empty_buffer.getNumFrames();
    int channels = empty_buffer.getNumChannels();
    EXPECT_THAT(samples, Eq(0));
    EXPECT_THAT(channels, Eq(0));
}

TEST_F(AAudioBuffer, CreatAnEmptyBufferWithSizeAndNumChannels)
{
    AudioBuffer<float> buffer(num_channels, num_samples);

    int samples = buffer.getNumFrames();
    int channels = buffer.getNumChannels();

    EXPECT_THAT(samples, Eq(num_samples));
    EXPECT_THAT(channels, Eq(num_channels));
}

TEST_F(AAudioBuffer, SetSizeChangesNumChannelsAndNumSamples)
{
    AudioBuffer<float> buffer;

    int new_num_channels = 2;
    int new_num_samples = 10;

    buffer.setSize(new_num_channels, new_num_samples);

    ASSERT_THAT(buffer.getNumChannels(), Eq(new_num_channels));
    ASSERT_THAT(buffer.getNumFrames(), Eq(new_num_samples));
}

TEST_F(AAudioBuffer, SetSizeWillClearTheKeepExistingContent)
{
    AudioBuffer<float> buffer(2, 5);
    buffer.getWritePointer(0)[0] = 1.0f;
    buffer.getWritePointer(1)[0] = 1.0f;

    int new_num_channels = 2;
    int new_num_samples = 10;
    buffer.setSize(new_num_channels, new_num_samples);

    ASSERT_THAT(buffer.getWritePointer(0)[0], FloatEq(0.0f));
    ASSERT_THAT(buffer.getWritePointer(1)[0], FloatEq(0.0f));
}

TEST_F(AAudioBuffer, AllocMemoryWhenCreateWithSize)
{
    AudioBuffer<float> buffer(num_channels, num_samples);

    float* wp = buffer.getWritePointer(0);
    const float* rp = buffer.getReadPointer(0);
    ASSERT_THAT(wp, Not(nullptr));
    ASSERT_THAT(rp, Not(nullptr));
}

TEST_F(AAudioBuffer, CreateWithDataReferTo)
{
    AudioBuffer<float> buffer(external_data.data(), num_channel_to_use, start_sample, num_samples);

    EXPECT_THAT(buffer.getNumFrames(), num_samples);
    EXPECT_THAT(buffer.getNumChannels(), num_channel_to_use);
    EXPECT_THAT(buffer.getWritePointer(0), Not(nullptr));
    EXPECT_THAT(buffer.getWritePointer(1), Not(nullptr));
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(233));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(233));
}

TEST_F(AAudioBuffer, CreateWithDataReferToWillShareTheSameMemory)
{

    AudioBuffer<float> buffer(external_data.data(), num_channel_to_use, start_sample, num_samples);

    left_data[233] = 1;
    right_data[233] = 1;
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(1));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(1));

    buffer.getWritePointer(0)[233] = 0;
    buffer.getWritePointer(1)[233] = 0;
    EXPECT_THAT(buffer.getReadPointer(0)[233], FloatEq(0));
    EXPECT_THAT(buffer.getReadPointer(1)[233], FloatEq(0));
}

TEST_F(AAudioBuffer, CopiesAnotherBufferSharedSameDataIfTheBufferUsingExternalBufferData)
{
    AudioBuffer<float> buffer(external_data.data(), num_channel_to_use, start_sample, num_samples);

    AudioBuffer<float> new_buffer(buffer);
    buffer.getWritePointer(0)[0] = 10;

    ASSERT_THAT(new_buffer.getWritePointer(0)[0], Eq(10));
}

TEST_F(AAudioBuffer, CopiesAnotherBufferHasOwnData)
{
    AudioBuffer<float> origin_buffer(2, 10);

    AudioBuffer<float> new_buffer(origin_buffer);
    origin_buffer.getWritePointer(0)[0] = 10;

    ASSERT_THAT(new_buffer.getWritePointer(0)[0], Not(10));
}

TEST_F(AAudioBuffer, MakeACopyWillCopyAllOfItsContent)
{
    AudioBuffer<float> buffer(external_data.data(), num_channel_to_use, start_sample, num_samples);

    AudioBuffer<float> new_buffer;
    new_buffer.copyFrom(buffer);

    buffer.getWritePointer(0)[0] = 10;

    ASSERT_THAT(new_buffer.getWritePointer(0)[0], Not(10));
}

TEST_F(AAudioBuffer, AssignAnotherBufferHasOwnData)
{
    AudioBuffer<float> origin_buffer(2, 10);

    AudioBuffer<float> new_buffer;
    new_buffer = origin_buffer;
    origin_buffer.getWritePointer(0)[0] = 10;

    ASSERT_THAT(new_buffer.getWritePointer(0)[0], Not(10));
}

TEST_F(AAudioBuffer, AllSamplesInAllChannelSetToZeroAfterClear)
{
    vector<float*> data_refet_to = {left_data, right_data};
    int num_channel_to_use = 2;
    int start_sample = 0;
    AudioBuffer<float> buffer(data_refet_to.data(), num_channel_to_use, start_sample, num_samples);

    buffer.clear();

    int arbitrary_index = 100;
    EXPECT_THAT(buffer.getReadPointer(0)[arbitrary_index], FloatEq(0));
    EXPECT_THAT(buffer.getReadPointer(1)[arbitrary_index], FloatEq(0));
}

