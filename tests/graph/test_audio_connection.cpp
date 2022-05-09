//
// Created by user on 4/28/22.
//
#include "libaa/graph/aa_audio_connection.h"
#include "libaa_testing/aa_mock_node.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AAudioConnection : public Test {
public:
    AAudioConnection()
        : port(nullptr) {}
    void SetUp() override {
        block = std::shared_ptr<AudioBlock>(
            new AudioBlock({{1, 2, 3, 4}, {5, 6, 7, 8}}));

        c = std::make_unique<AudioConnection>(n, up_port_index,
                                              down_port_index);
    }

    std::shared_ptr<MockNode> n = std::make_shared<MockNode>();
    const int up_port_index = 1;
    const int down_port_index = 2;

    std::unique_ptr<AudioConnection> c;
    std::shared_ptr<AudioBlock> block;
    std::shared_ptr<AudioPort> port;
};

TEST_F(AAudioConnection, InitWithUpsteamNodeAndPort) {
    c = std::make_unique<AudioConnection>(n, up_port_index, down_port_index);
}

TEST_F(AAudioConnection, CanGetUpStreamNode) {
    ASSERT_THAT(c->upstream_node.get(), Eq(n.get()));
}

TEST_F(AAudioConnection, CanGetUpStreamPortIndex) {
    ASSERT_THAT(c->upstream_port_index, Eq(up_port_index));
}

TEST_F(AAudioConnection, CanGetDownStreamPortIndex) {
    ASSERT_THAT(c->downstream_port_index, Eq(down_port_index));
}

TEST_F(AAudioConnection, PullWillTriggerUpstreamNodeToPull) {
    AudioPort audio_port(block);

    EXPECT_CALL(*n, pullAudioPort(up_port_index))
        .WillOnce(ReturnRef(audio_port));

    auto &p = c->pull();

    ASSERT_THAT(p.getNumberChannels(), Eq(block->buffer.getNumberChannels()));
    ASSERT_THAT(p.getNumberFrames(), Eq(block->buffer.getNumberFrames()));
    ASSERT_THAT(p.getChannelData(0)[0], Eq(1));
    ASSERT_THAT(p.getChannelData(1)[0], Eq(5));
}