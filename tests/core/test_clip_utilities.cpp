//
// Created by user on 6/7/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_clip_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AClipUtilities : public Test {
public:
    auto makeScopeWavFile(const std::string &file_uri) {
        auto scope_file = std::unique_ptr<ScopeWaveFile>(new ScopeWaveFile(
            file_uri,
            44100,
            {{1, 1, 1}, {2, 2, 2}},
            16));
        return scope_file;
    }
    std::string mock_file_uri = "clip_utilities_test.wav";
};

TEST_F(AClipUtilities, ClipSerializationStringContainsLoopingState) {
    bool looping = false;
    auto scope_file = makeScopeWavFile(mock_file_uri);
    auto clip = ClipFactory::buildClip(mock_file_uri, looping);

    auto json = ClipUtilities::serialize(clip.get());

    ASSERT_THAT(json["looping"].get<bool>(), Eq(looping));
}

TEST_F(AClipUtilities, ClipSerializationStringContainsAudioFileUri) {
    bool looping = false;
    auto scope_file = makeScopeWavFile(mock_file_uri);
    auto clip = ClipFactory::buildClip(mock_file_uri, looping);

    auto json = ClipUtilities::serialize(clip.get());

    ASSERT_THAT(json["audio_file_uri"].get<std::string>(), Eq(mock_file_uri));
}