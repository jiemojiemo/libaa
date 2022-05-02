//
// Created by user on 4/28/22.
//

#include "libaa/graph/aa_audio_processor_node.h"
#include <gmock/gmock.h>

using namespace testing;

namespace libaa {

class MockProcessor : public IAudioProcessor {
public:
    ~MockProcessor() override = default;
    MockProcessor() {
        ON_CALL(*this, getName).WillByDefault(Return("MockProcessor"));
        ON_CALL(*this, getLatencySamples).WillByDefault(Return(0));
        ON_CALL(*this, getTailLengthSamples).WillByDefault(Return(0));
    }

    MOCK_METHOD(std::string, getName, (), (override, const));
    MOCK_METHOD(void, processBlock, (AudioBlock *, AudioBlock *), (override));
    MOCK_METHOD(const AudioProcessorParameters *, getParameters, (),
                (override, const));
    MOCK_METHOD(void, prepareToPlay, (float, int), (override));
    MOCK_METHOD(int, getLatencySamples, (), (override, const, noexcept));
    MOCK_METHOD(int, getTailLengthSamples, (), (override, const, noexcept));
};

} // namespace libaa

using namespace libaa;

class AProcessorNode : public Test {
public:
    void SetUp() override {
        proc = std::make_shared<NiceMock<MockProcessor>>();
        up_proc = std::make_shared<NiceMock<MockProcessor>>();
        upstream_node = std::make_shared<ProcessorNode>(up_proc);

        params.pushFloatParameter("A", 0, 0, 1.0);
        params.pushFloatParameter("B", 0, 0, 1.0);
        params.pushFloatParameter("C", 0, 0, 1.0);
        params.pushFloatParameter("D", 0, 0, 1.0);
    }
    std::shared_ptr<MockProcessor> proc;
    std::shared_ptr<MockProcessor> up_proc;
    std::shared_ptr<ProcessorNode> upstream_node;
    AudioBlock block{{{1, 1, 1}, {2, 2, 2}}};

    AudioProcessorParameters params;
    const float sr = 44100;
    const int max_block_size = 128;
};

TEST_F(AProcessorNode, CanInitWithInputsAndOutputAndProcessor) {
    ProcessorNode node(proc, {2}, {2});

    ASSERT_THAT(proc.use_count(), Eq(2));
}

TEST_F(AProcessorNode, InitWithChannelsReallocateInternalBuffers) {
    auto input_channels = {2};
    auto output_channels = {2, 2};
    ProcessorNode node(proc, input_channels, output_channels);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(4));
}

TEST_F(AProcessorNode, CanInitWithProcessor) {
    ProcessorNode node(proc);

    ASSERT_THAT(proc.use_count(), Eq(2));
}

TEST_F(AProcessorNode, InitWithProcessorHasDefaultInputAndOutputChannels) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(2));
}

TEST_F(AProcessorNode, InitWithProcessReallocateParameterChanges) {

    auto num_params = params.size();

    EXPECT_CALL(*proc, getParameters).WillRepeatedly(Return(&params));

    ProcessorNode node(proc);
    ASSERT_THAT(node.getInputBlock()->param_changes.getNumParameters(),
                Eq(num_params));
}

TEST_F(AProcessorNode, HasDefalutOneInputAudioPort) {
    ProcessorNode node(proc);

    auto ports = node.getAudioInputPorts();
    ASSERT_THAT(ports.size(), Eq(1));
    ASSERT_THAT(ports[0].getNumberChannels(), Eq(2));
}

TEST_F(AProcessorNode, HasDefalutOneOutputAudioPort) {
    ProcessorNode node(proc);

    auto ports = node.getAudioOutputPorts();
    ASSERT_THAT(ports.size(), Eq(1));
    ASSERT_THAT(ports[0].getNumberChannels(), Eq(2));
}

TEST_F(AProcessorNode, InitWithChannelsHasMultipleInputAudioPort) {
    auto input_channels = {2, 2};
    auto output_channels = {1};
    ProcessorNode node(proc, input_channels, output_channels);

    auto input_ports = node.getAudioInputPorts();
    auto output_ports = node.getAudioOutputPorts();

    ASSERT_THAT(input_ports.size(), Eq(input_channels.size()));
    ASSERT_THAT(input_ports[0].getNumberChannels(), Eq(2));
    ASSERT_THAT(input_ports[1].getNumberChannels(), Eq(2));

    ASSERT_THAT(output_ports.size(), Eq(output_channels.size()));
    ASSERT_THAT(output_ports[0].getNumberChannels(), Eq(1));
}

TEST_F(AProcessorNode, AddInputPortWillReallocateInputBuffer) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));

    node.addAudioInputPort(2);
    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(4));
}

TEST_F(AProcessorNode, AddAudioInputPortIncreaseTheAudioPorts) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getAudioInputPorts().size(), Eq(1));

    node.addAudioInputPort(2);
    ASSERT_THAT(node.getAudioInputPorts().size(), Eq(2));
}

TEST_F(AProcessorNode, DefaultIDIsEmpty) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getNodeID(), Eq(""));
}

TEST_F(AProcessorNode, CanSetNodeId) {
    auto id = "ABC";
    ProcessorNode node(proc);

    node.setNodeID(id);

    ASSERT_THAT(node.getNodeID(), Eq(id));
}

TEST_F(AProcessorNode, PrepareToPlayWillReallocateInputAudioBuffer) {
    ProcessorNode node(proc);
    node.prepareToPlay(sr, max_block_size);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberFrames(),
                Eq(max_block_size));
}

TEST_F(AProcessorNode, PrepareToPlayWillReallocateOutputAudioBuffer) {
    ProcessorNode node(proc);
    node.prepareToPlay(sr, max_block_size);

    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberFrames(),
                Eq(max_block_size));
}

TEST_F(AProcessorNode, PrepareToPlayWillPrepareTheProcessor) {
    ProcessorNode node(proc);

    EXPECT_CALL(*proc, prepareToPlay(sr, max_block_size)).Times(1);

    node.prepareToPlay(sr, max_block_size);
}

TEST_F(AProcessorNode, HasNoAudioConnectionWhenInit) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getUpstreamAudioConnections().size(), Eq(0));
}

TEST_F(AProcessorNode, CanAddUpstreamAudioConnection) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};

    node.addUpstreamAudioConnection(connection);
}

TEST_F(AProcessorNode, AddAudioConnectionIncreasesTheConnectionSize) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};

    node.addUpstreamAudioConnection(connection);

    ASSERT_THAT(node.getUpstreamAudioConnections().size(), Eq(1));
}

TEST_F(AProcessorNode, AddConnectionThrowsIfUpstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int upstream_port_index = 1;
    AudioConnection connection{upstream_node, 1, 0};

    ASSERT_THAT(upstream_port_index,
                Ge(upstream_node->getAudioOutputPorts().size()));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, AddConnectionThrowsIfDownstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int downstream_port_index = 1;
    AudioConnection connection{upstream_node, 0, downstream_port_index};

    ASSERT_THAT(downstream_port_index, Ge(node.getAudioInputPorts().size()));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, AddConnectionThrowsChannelsMismatch) {
    ProcessorNode node(proc, {1}, {2});
    AudioConnection connection{upstream_node, 0, 0};

    ASSERT_THAT(upstream_node->getAudioOutputPorts()[0].getNumberChannels(),
                Not(node.getAudioInputPorts()[0].getNumberChannels()));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, HasProcessReturnsFalseWhenInit) {
    ProcessorNode node(proc);

    ASSERT_FALSE(node.hasProcessed());
}

TEST_F(AProcessorNode, HasProcessReturnsTrueAfterPullAudioPort) {
    ProcessorNode node(proc);

    node.pullAudioPort(0);

    ASSERT_TRUE(node.hasProcessed());
}

TEST_F(AProcessorNode, PrepareForNextBlockResetTheProcessState) {
    ProcessorNode node(proc);
    node.pullAudioPort(0);
    ASSERT_TRUE(node.hasProcessed());

    node.prepareForNextBlock();

    ASSERT_FALSE(node.hasProcessed());
}

TEST_F(AProcessorNode, PullAudioPortWillPullUpstreamAudio) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};
    node.addUpstreamAudioConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock)
        .WillOnce([this](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&this->block.buffer,
                                    this->block.buffer.getNumberChannels(),
                                    this->block.buffer.getNumberFrames(), 0, 0);
        });

    EXPECT_CALL(*proc, processBlock)
        .WillOnce([](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&input->buffer);
        });

    AudioPort &port = node.pullAudioPort(0);

    ASSERT_THAT(port.getChannelData(0)[0], Eq(1));
    ASSERT_THAT(port.getChannelData(1)[0], Eq(2));
}

TEST_F(AProcessorNode, PullAudioPortReturnsLastResultIfHasProcessed) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};
    node.addUpstreamAudioConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock).Times(1);
    EXPECT_CALL(*proc, processBlock).Times(1);

    AudioPort &port_first_time = node.pullAudioPort(0);
    AudioPort &port_second_time = node.pullAudioPort(0);

    ASSERT_THAT(port_first_time.getNumberChannels(),
                Eq(port_second_time.getNumberChannels()));
    ASSERT_THAT(port_first_time.getChannelData(0),
                Eq(port_second_time.getChannelData(0)));
}