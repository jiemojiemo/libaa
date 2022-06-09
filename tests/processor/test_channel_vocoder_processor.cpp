//
// Created by user on 6/7/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_clip_utilities.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/processor/aa_channel_vocoder_processor.h"
#include "libaa/processor/aa_processor_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AChannelVocoderProcessor : public Test {
public:
    auto makeEmptyClip() {
        return std::make_shared<Clip>(AudioBlock{}, true);
    }

    auto makeFileClip(const std::string &file_uri) {
        std::shared_ptr<Clip> c = ClipFactory::buildClip(file_uri, true);
        return c;
    }

    std::string mock_file_uri = "clip_utilities_test.wav";
    float sample_rate = 4;
    int max_block_size = 4;
};

TEST_F(AChannelVocoderProcessor, InitWithClip) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    ASSERT_THAT(clip.use_count(), Eq(2));
}

TEST_F(AChannelVocoderProcessor, CanInitWithFinder) {
    auto finder = std::make_shared<ResourcePathFinder>();
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip, finder);

    ASSERT_THAT(finder.use_count(), Eq(2));
}

TEST_F(AChannelVocoderProcessor, CanReportCorrectName) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    ASSERT_THAT(proc.getName(), Eq("ChannelVocoder"));
}

TEST_F(AChannelVocoderProcessor, HasParameters) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    ASSERT_THAT(proc.getParameters(), NotNull());
}

TEST_F(AChannelVocoderProcessor, NumberBandParameterAsExpected) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    int param_index = 0;
    auto param = proc.getParameters()->get(param_index);

    ASSERT_THAT(param.getParameterID(), Eq(param_index));
    ASSERT_THAT(param.getParameterName(), Eq("Number Band"));
    ASSERT_THAT(param.getParameterType(), Eq(ParameterType::kInt));
    ASSERT_THAT(param.getPlainValue(), Eq(32));
    ASSERT_THAT(param.getMinPlainValue(), Eq(16));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(256));
}

TEST_F(AChannelVocoderProcessor, StateContainsClipState) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    auto state = proc.getState();
    auto json = JsonUtilities::binaryDataToJson(state);
    auto expected_json = ClipUtilities::serialize(clip.get());

    ASSERT_THAT(json["clip"], Eq(expected_json));
}

TEST_F(AChannelVocoderProcessor, SetStateWillRebuildClip) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    auto scope_file = makeScopeWavFile(mock_file_uri);
    auto state_json = JsonUtilities::binaryDataToJson(proc.getState());
    state_json["clip"]["audio_file_uri"] = mock_file_uri;
    auto new_state = JsonUtilities::jsonToBinaryData(state_json);

    proc.setState(new_state.data(), new_state.size());

    ASSERT_THAT(proc.getCarrierClip()->getAudioFileUri(), Eq(mock_file_uri));
    ASSERT_THAT(proc.getCarrierClip()->getNumberFrames(), Eq(3));
}

TEST_F(AChannelVocoderProcessor, SetStateThrowsIfCannotOpenCarrierFile) {
    auto clip = makeEmptyClip();
    ChannelVocoderProcessor proc(clip);

    auto state_json = JsonUtilities::binaryDataToJson(proc.getState());
    state_json["clip"]["audio_file_uri"] = "invalid_file.wav";
    auto new_state = JsonUtilities::jsonToBinaryData(state_json);

    ASSERT_ANY_THROW(proc.setState(new_state.data(), new_state.size()));
}

TEST_F(AChannelVocoderProcessor, SetStateUpdatesParameters) {
    auto scope_file = makeScopeWavFile(mock_file_uri);
    auto clip = makeFileClip(mock_file_uri);
    ChannelVocoderProcessor proc(clip);

    float num_band = 64;
    auto state_json = JsonUtilities::binaryDataToJson(proc.getState());
    state_json["parameters"]["Number Band"] = num_band;
    auto new_state_str = nlohmann::to_string(state_json);

    proc.setState((uint8_t *)new_state_str.data(), new_state_str.size());

    float value = proc.getParameters()->get(0).getPlainValue();
    ASSERT_THAT(value, Eq(num_band));
}

TEST_F(AChannelVocoderProcessor, ProcessAsExpected) {
    AudioBlock block{{{1, 0, 0, 0}, {1, 0, 0, 0}}, 1};
    auto scope_file = makeScopeWavFile(mock_file_uri);
    std::shared_ptr<Clip> clip = ClipFactory::buildClip(mock_file_uri, true);
    ChannelVocoderProcessor proc(clip);
    proc.prepareToPlay(sample_rate, max_block_size);

    proc.processBlock(&block, &block);

    AudioBlock expected{{{0, 0, 0, 0}, {0, 0, 0, 0}}};
    ASSERT_THAT(block.buffer, Eq(expected.buffer));
}

TEST_F(AChannelVocoderProcessor, ParameterChangedIfPushAParameterChangePoint) {
    AudioBlock block{{{1, 0, 0, 0}, {1, 0, 0, 0}}, 1};
    auto scope_file = makeScopeWavFile(mock_file_uri);
    std::shared_ptr<Clip> clip = ClipFactory::buildClip(mock_file_uri, true);
    ChannelVocoderProcessor proc(clip);
    proc.prepareToPlay(sample_rate, max_block_size);

    float expected_val = 0.5;
    block.param_changes.push(0, {0, 0, expected_val});
    proc.processBlock(&block, &block);

    ASSERT_THAT(proc.getParameters()->get(0).getNormalizedValue(), Eq(expected_val));
}