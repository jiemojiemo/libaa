//
// Created by user on 5/8/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/core/aa_resource_path_finder.h"
#include "libaa/processor/aa_source_processor.h"
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

    ASSERT_THAT(proc.getName(), Eq("Source"));
}

TEST_F(ASourceProcessor, HasNoParameters) {
    SourceProcessor proc;

    ASSERT_THAT(proc.getParameters(), IsNull());
}

TEST_F(ASourceProcessor, CanInitWithAClip) {
    AudioBlock block{{{1, 2, 3},
                      {4, 5, 6}}};
    auto clip = std::make_shared<Clip>(block, true);
    SourceProcessor proc{clip};
}

TEST_F(ASourceProcessor, CanInitWithFinder) {
    auto finder = std::make_shared<ResourcePathFinder>();
    auto clip = std::make_shared<Clip>(AudioBlock{}, true);
    SourceProcessor proc{clip, finder};
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

TEST_F(ASourceProcessor, ProcessGenerateLoopingSamplesIfClipIsLooping) {
    auto clip = std::make_shared<Clip>(AudioBlock{{{1, 2, 3}, {4, 5, 6}}},
                                       true);
    SourceProcessor proc{clip};
    AudioBlock block{2, 3, 0};
    block.context.play_head_sample_index = 2;

    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);

    AudioBufferX<float> expected{{{3, 1, 2},
                                  {6, 4, 5}}};
    ASSERT_THAT(block.buffer, Eq(expected));
}

TEST_F(ASourceProcessor, StateStringContainsAudioFileUri) {
    auto mock_audio_file_uri = "abc.wav";
    auto clip = std::make_shared<Clip>(
        AudioBlock{{{1, 2, 3}, {4, 5, 6}}},
        true,
        mock_audio_file_uri);

    SourceProcessor proc{clip};
    auto state = proc.getState();
    auto json = JsonUtilities::binaryDataToJson(state);

    ASSERT_THAT(json["clip"]["audio_file_uri"].get<std::string>(), Eq(mock_audio_file_uri));
}

TEST_F(ASourceProcessor, StateStringContainsClipLoopingState) {
    bool is_looping = true;
    auto clip = std::make_shared<Clip>(AudioBlock{}, is_looping);

    SourceProcessor proc{clip};
    auto state = proc.getState();
    auto json = JsonUtilities::binaryDataToJson(state);

    std::cout << json << std::endl;

    ASSERT_THAT(json["clip"]["looping"].get<bool>(), Eq(is_looping));
}

TEST_F(ASourceProcessor, SetStateWillReadFileSamples) {
    auto mock_audio_file_uri = "abc.wav";
    ScopeWaveFile scope_file(mock_audio_file_uri, 44100, {{1, 1, 1}, {2, 2, 2}}, 16);
    auto clip = std::make_shared<Clip>(
        AudioBlock{},
        true,
        mock_audio_file_uri);

    SourceProcessor proc{clip};
    auto state = proc.getState();
    proc.setState(state.data(), state.size());

    AudioBlock block{2, 3, 0};
    proc.prepareToPlay(sample_rate, max_block_size);
    proc.processBlock(&block, &block);
    AudioBufferX<float> expected{{{1, 1, 1}, {2, 2, 2}}};

    ASSERT_THAT(block.buffer, Eq(expected));
}