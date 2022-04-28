//
// Created by user on 4/28/22.
//
#include "libaa/graph/aa_audio_port.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AAudioPort : public Test {
public:
    void SetUp() override {
        block =
            std::shared_ptr<AudioBlock>(new AudioBlock({{1, 2, 3}, {4, 5, 6}}));
    }

    std::shared_ptr<AudioBlock> block;
};

TEST_F(AAudioPort, CanInitWithAudioBlock) {
    auto port = std::make_unique<AudioPort>(block);
}

TEST_F(AAudioPort, CanGetChannelDataFromBlock) {
    auto port = std::make_unique<AudioPort>(block);

    ASSERT_THAT(port->getChannelData(0)[0], Eq(1));
    ASSERT_THAT(port->getChannelData(1)[0], Eq(4));
}

TEST_F(AAudioPort, CanGetNumChannels) {
    auto port = std::make_unique<AudioPort>(block);

    ASSERT_THAT(port->getNumberChannels(),
                Eq(block->buffer.getNumberChannels()));
}

TEST_F(AAudioPort, CanGetNumSamples) {
    auto port = std::make_unique<AudioPort>(block);

    ASSERT_THAT(port->getNumberFrames(), Eq(block->buffer.getNumberFrames()));
}

TEST_F(AAudioPort, CanInitWithAudioBlockAndChannels) {
    int num_channels = 1;
    auto port = std::make_unique<AudioPort>(block, num_channels);

    ASSERT_THAT(port->getNumberChannels(), num_channels);
}

TEST_F(AAudioPort, GetChannelReturnNullIfOutofRange) {
    int num_channels = 1;
    auto port = std::make_unique<AudioPort>(block, num_channels);

    ASSERT_THAT(port->getChannelData(2), IsNull());
}