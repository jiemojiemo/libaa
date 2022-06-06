//
// Created by user on 6/6/22.
//
#include "libaa/core/aa_clip.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AClip : public Test {
public:
    AudioBlock block{{{1, 1.1, 1.2}, {2, 2.1, 2.2}}};
    bool is_looping = true;
};

TEST_F(AClip, InitWithBlcokAndLoopingFlag) {
    Clip clip(block, true);
}

TEST_F(AClip, CanGetLooping) {
    Clip clip(block, is_looping);

    ASSERT_THAT(clip.isLooping(), Eq(is_looping));
}

TEST_F(AClip, CanReturnNumberOfAudioBuffer) {
    Clip clip(block, is_looping);

    ASSERT_THAT(clip.getNumberChannels(), Eq(block.buffer.getNumberChannels()));
}

TEST_F(AClip, CanReadSamples) {
    Clip clip(block, is_looping);

    float x = 0.0f;
    x = clip.readSample(0, 0);
    ASSERT_THAT(x, Eq(1));
    x = clip.readSample(1, 0);
    ASSERT_THAT(x, Eq(2));
}
TEST_F(AClip, CanReadSamplesIfLooping) {
    Clip clip(block, true);

    float x = 0.0f;
    x = clip.readSample(0, 3);
    ASSERT_THAT(x, Eq(1.0f));
    x = clip.readSample(0, 4);
    ASSERT_THAT(x, Eq(1.1f));
    x = clip.readSample(1, 3);
    ASSERT_THAT(x, Eq(2.0f));
    x = clip.readSample(1, 4);
    ASSERT_THAT(x, Eq(2.1f));
}

TEST_F(AClip, DefaultFileUriIsEmpty) {
    Clip clip(block, true);

    ASSERT_TRUE(clip.getAudioFileUri().empty());
}

TEST_F(AClip, CanInitWithAudioFileUri) {
    auto audio_file_uri = "abc.wav";
    Clip clip(block, true, audio_file_uri);

    ASSERT_THAT(clip.getAudioFileUri(), Eq(audio_file_uri));
}

TEST_F(AClip, CanGetTotalSamples) {
    Clip clip(block, true);

    ASSERT_THAT(clip.getNumberFrames(), Eq(block.buffer.getNumberFrames()));
}