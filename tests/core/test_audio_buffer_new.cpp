
//
// Created by William.Hua on 2021/7/25.
//

#include "aa_testing_helper.h"
#include "libaa/core/aa_audio_buffer_new.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace std;
using namespace libaa;

class AAudioBufferNew : public Test {
public:
    void SetUp() override {}

    int num_channels = 2;
    int num_frames = 2;
    list<vector<float>> channel_data = {{1, 2, 3}, {4, 5, 6}};
};

TEST_F(AAudioBufferNew, CanContructWithSize) {
    AudioBufferX<float> audio_buffer(num_channels, num_frames);
}

TEST_F(AAudioBufferNew, DefaultConstuctWithEmptyData) {
    AudioBufferX<float> audio_buffer;

    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(0));
    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(0));
}

TEST_F(AAudioBufferNew, ConstructWithSizeWillFillZerosToBuffer) {
    AudioBufferX<float> audio_buffer(num_channels, num_frames);

    for (int c = 0; c < num_channels; ++c) {
        for (int i = 0; i < num_frames; ++i) {
            ASSERT_THAT(audio_buffer.getReadPointer(c)[i], Eq(0.0f));
        }
    }
}

TEST_F(AAudioBufferNew, CanGetNumberOfChannels) {
    AudioBufferX<float> audio_buffer(num_channels, num_frames);
    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(num_channels));
}

TEST_F(AAudioBufferNew, CanGetNumberOfFrames) {
    AudioBufferX<float> audio_buffer(num_channels, num_frames);
    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(num_channels));
}

TEST_F(AAudioBufferNew, CanConstructWithInitialzerList) {
    AudioBufferX<float> audio_buffer({{1, 2, 3}, {4, 5, 6}});
}

TEST_F(AAudioBufferNew, ConstructWithListCanGetNumChannels) {
    AudioBufferX<float> audio_buffer({{1, 2, 3}, {4, 5, 6}});

    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(2));
}

TEST_F(AAudioBufferNew, CanGetNumberOfFramesEqualsTheMaxFramesOfAllChannel) {
    AudioBufferX<float> audio_buffer({{1, 2, 3}, {2}});

    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(3));
}

TEST_F(AAudioBufferNew, DoCopyWhenConstructWithList) {
    AudioBufferX<float> audio_buffer({{1, 2, 3}, {2}});

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], Eq(1.0));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[1], Eq(2.0));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[2], Eq(3.0));

    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], Eq(2));
}

TEST_F(AAudioBufferNew, CanCopyFromOtherBuffer) {
    AudioBufferX<float> input_buffer({{1, 2, 3}, {4, 5, 6}});
    AudioBufferX<float> output_buffer(input_buffer.getNumberChannels(),
                                      input_buffer.getNumberFrames());

    output_buffer.copyFrom(&input_buffer);

    ASSERT_THAT(output_buffer, Eq(input_buffer));
}

TEST_F(AAudioBufferNew, CanCopyFromOtherBufferWithOffset) {
    AudioBufferX<float> input_buffer({{1, 2, 3}, {4, 5, 6}});
    AudioBufferX<float> output_buffer(input_buffer.getNumberChannels(),
                                      input_buffer.getNumberFrames());

    output_buffer.copyFrom(&input_buffer, 1, 2, 1, 1);

    ASSERT_THAT(output_buffer.getReadPointer(0)[1], Eq(2));
    ASSERT_THAT(output_buffer.getReadPointer(0)[2], Eq(3));
}

TEST_F(AAudioBufferNew, CanResize) {
    AudioBufferX<float> audio_buffer(2, 3);

    int new_channels = 3;
    int new_num_frames = 5;
    audio_buffer.resize(new_channels, new_num_frames);

    ASSERT_THAT(audio_buffer.getNumberChannels(), Eq(new_channels));
    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(new_num_frames));
}

TEST_F(AAudioBufferNew, CanCopyFromFloatArrays) {
    AudioBufferX<float> audio_buffer(2, 3);
    float samples[2][3]{{1, 1, 1},
                        {
                            2,
                            2,
                            2,
                        }};
    float *data_refer_to[2] = {samples[0], samples[1]};

    audio_buffer.copyFrom(data_refer_to, 2, 3, 0, 0);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], Eq(1));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], Eq(2));
}

TEST_F(AAudioBufferNew, CanCopyToOtherFloatArrays) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};
    float samples[2][3];
    float *data_refer_to[2] = {samples[0], samples[1]};

    audio_buffer.copyTo(data_refer_to, 2, 3, 0, 0);

    ASSERT_THAT(samples[0][0], Eq(1));
    ASSERT_THAT(samples[1][0], Eq(4));
}

TEST_F(AAudioBufferNew, GetNullWriterPointerIfChannelsOutputOfRange) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};

    ASSERT_THAT(audio_buffer.getWriterPointer(3), Eq(nullptr));
}

TEST_F(AAudioBufferNew, GetNullReadPointerIfChannelsOutputOfRange) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};

    ASSERT_THAT(audio_buffer.getReadPointer(3), Eq(nullptr));
}

TEST_F(AAudioBufferNew, ResizeFramesReallocateInternalBuffer) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};
    int new_frames = 10;

    audio_buffer.resizeFrames(new_frames);

    ASSERT_THAT(audio_buffer.getNumberFrames(), Eq(new_frames));
}

TEST_F(AAudioBufferNew, CanApplyGainWithSpecifyChannel) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};
    float gain = 0.5;

    audio_buffer.applyGain(0, 0, audio_buffer.getNumberFrames(), gain);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], Eq(0.5f));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], Eq(4.0f));
}

TEST_F(AAudioBufferNew, CanApplyGainToAllChannels) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};
    float gain = 0.5;

    audio_buffer.applyGain(0, audio_buffer.getNumberFrames(), gain);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], Eq(0.5f));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], Eq(2.0f));
}

TEST_F(AAudioBufferNew, ApplyGainRampWithChannel) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {4, 5, 6}}};
    float start_gain = 0;
    float end_gain = 0.5;

    audio_buffer.applyGainRamp(0, 0, audio_buffer.getNumberFrames(), start_gain, end_gain);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], FloatEq(0.0f));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[1], FloatNear(0.333333f, 1e-6));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[2], FloatEq(1.0f));
}

TEST_F(AAudioBufferNew, ApplyGainRampToAllChannel) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {1, 2, 3}}};
    float start_gain = 0;
    float end_gain = 0.5;

    audio_buffer.applyGainRamp(0, audio_buffer.getNumberFrames(), start_gain, end_gain);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], FloatEq(0.0f));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[1], FloatNear(0.333333f, 1e-6));
    ASSERT_THAT(audio_buffer.getReadPointer(0)[2], FloatEq(1.0f));

    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], FloatEq(0.0f));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[1], FloatNear(0.333333f, 1e-6));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[2], FloatEq(1.0f));
}

TEST_F(AAudioBufferNew, ApplyGainRampWhenStartGainSameWithEndGainUseStaticGain) {
    AudioBufferX<float> audio_buffer{{{1, 2, 3}, {1, 2, 3}}};
    float start_gain = 0.5;
    float end_gain = 0.5;

    audio_buffer.applyGainRamp(0, audio_buffer.getNumberFrames(), start_gain, end_gain);

    ASSERT_THAT(audio_buffer.getReadPointer(0)[0], FloatEq(0.5f));
    ASSERT_THAT(audio_buffer.getReadPointer(1)[0], FloatEq(0.5f));
}

