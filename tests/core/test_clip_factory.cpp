//
// Created by user on 6/6/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_resource_path_finder.h"
#include <gmock/gmock.h>
using namespace testing;

using namespace libaa;
class AClipFactory : public Test {
public:
    std::string mock_wave_file_uri = "abc.wav";
    int sample_rate = 44100;
    std::vector<std::vector<float>> data = {{1, 1, 1},
                                            {2, 2, 2}};
};

TEST_F(AClipFactory, BuildClipContainsAudioFileUri) {
    ScopeWaveFile scope_wave(mock_wave_file_uri, sample_rate, data, 16);
    auto clip = ClipFactory::buildClip(mock_wave_file_uri, true);

    ASSERT_THAT(clip->getAudioFileUri(), Eq(mock_wave_file_uri));
}

TEST_F(AClipFactory, BuildClipContainsLoopingFlag) {
    bool is_looping = true;
    ScopeWaveFile scope_wave(mock_wave_file_uri, sample_rate, data, 16);
    auto clip = ClipFactory::buildClip(mock_wave_file_uri, is_looping);

    ASSERT_THAT(clip->isLooping(), Eq(is_looping));
}

TEST_F(AClipFactory, BuildClipReloadAudioFile) {
    ScopeWaveFile scope_wave(mock_wave_file_uri, sample_rate, data, 16);
    auto clip = ClipFactory::buildClip(mock_wave_file_uri, true);

    ASSERT_THAT(clip->getNumberChannels(), Eq(data.size()));
    ASSERT_THAT(clip->getNumberFrames(), Eq(data[0].size()));
    ASSERT_THAT(clip->readSample(0, 0), Eq(1));
    ASSERT_THAT(clip->readSample(1, 0), Eq(2));
}

TEST_F(AClipFactory, ReturnNullIfCannotOpenAudioUri) {
    auto clip = ClipFactory::buildClip("invalid_path.wav", true);

    ASSERT_THAT(clip, IsNull());
}

TEST_F(AClipFactory, CanBuildWithResourceFinder) {
    auto search_dir = "./abc/";
    ResourcePathFinder finder;
    finder.append(search_dir);
    mkdir(search_dir, S_IRWXU);

    ScopeWaveFile scope_wave(search_dir + mock_wave_file_uri, sample_rate, data, 16);
    auto clip = ClipFactory::buildClip(mock_wave_file_uri, true, &finder);

    ASSERT_THAT(clip->getNumberChannels(), Eq(data.size()));
    ASSERT_THAT(clip->getNumberFrames(), Eq(data[0].size()));
    ASSERT_THAT(clip->readSample(0, 0), Eq(1));
    ASSERT_THAT(clip->readSample(1, 0), Eq(2));
}