//
// Created by user on 5/7/22.
//
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_graph_node.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa/processor/aa_source_processor.h"
#include "libaa_testing/aa_mock_node.h"
#include "libaa_testing/aa_mock_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AGraphNode : public Test {
public:
    void SetUp() override {}

    std::shared_ptr<MockNode> node0 = std::make_shared<MockNode>();
    std::shared_ptr<MockNode> node1 = std::make_shared<MockNode>();
    std::shared_ptr<MockNode> upstream_node = std::make_shared<MockNode>();
    std::vector<std::shared_ptr<INode>> nodes{node0, node1};
    AudioBlock block{{{1, 1, 1}, {2, 2, 2}}};

    float sample_rate = 44100;
    int max_block_size = 128;
};

TEST_F(AGraphNode, CanInitWithNodes) {
    GraphNode node{nodes, {}, {}};

    ASSERT_THAT(node.getAllNodes().size(), Eq(2));
}

TEST_F(AGraphNode, CanInitWithNodesAndPorts) {
    GraphNode::InputPortNodeConnections input_audio_ports{{{node0, 0}},
                                                          {{node1, 0}}};
    GraphNode::OutputPortNodeConnections output_audio_ports{{node0, 0}};
    GraphNode::InputPortNodeConnections input_pc_ports{{{node0, 0}}};
    GraphNode::OutputPortNodeConnections output_pc_ports{{node0, 0}};

    GraphNode node{nodes, input_audio_ports, input_pc_ports, output_audio_ports, output_pc_ports};

    ASSERT_THAT(node.getInputAudioPortConnections().size(), Eq(2));
    ASSERT_THAT(node.getOutputAudioPortConnections().size(), Eq(1));
    ASSERT_THAT(node.getInputParameterChangePortConnections().size(), Eq(1));
    ASSERT_THAT(node.getOutputParameterChangePortConnections().size(), Eq(1));
}

TEST_F(AGraphNode, AudioInputPortSizeIsSameWithInputPortConnectionSize) {
    GraphNode::InputPortNodeConnections input_audio_ports{{{node0, 0}},
                                                          {{node1, 0}}};
    GraphNode::InputPortNodeConnections input_pc_ports{{{node0, 0}}};

    GraphNode node{nodes, input_audio_ports, input_pc_ports, {}, {}};

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(input_audio_ports.size()));
    ASSERT_THAT(node.getParameterChangeInputPortSize(), Eq(input_pc_ports.size()));
}

TEST_F(AGraphNode, AudioOutputPortSizeIsSameWithOutputPortConnectionSize) {
    GraphNode::OutputPortNodeConnections output_audio_ports{{node0, 0}};
    GraphNode::OutputPortNodeConnections output_pc_ports{{node0, 0}, {node1, 0}};

    GraphNode node{nodes, {}, {}, output_audio_ports, output_pc_ports};

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(output_audio_ports.size()));
    ASSERT_THAT(node.getParameterChangeOutputPortSize(), Eq(output_pc_ports.size()));
}

TEST_F(AGraphNode, CanGetNumberOfChannelsInInputPort) {
    GraphNode::InputPortNodeConnections input_audio_ports{{{node0, 0}},
                                                          {{node1, 0}}};

    GraphNode node{nodes, input_audio_ports, {}};

    ASSERT_THAT(node.getAudioInputPortChannels(0),
                Eq(node0->getAudioOutputPortChannels(0)));
    ASSERT_THAT(node.getAudioInputPortChannels(1),
                Eq(node1->getAudioOutputPortChannels(0)));
}

TEST_F(AGraphNode, GetTheFirstNodeNumberOfChannelsIfThereAreMultipleInputPort) {
    GraphNode::InputPortNodeConnections input_audio_ports{
        {{node0, 1}, {node1, 0}}};

    GraphNode node{nodes, input_audio_ports, {}};

    int expected_num_channels = 2;
    EXPECT_CALL(*node0, getAudioInputPortChannels(1))
        .WillOnce(Return(expected_num_channels));

    ASSERT_THAT(node.getAudioInputPortChannels(0), Eq(expected_num_channels));
}

TEST_F(AGraphNode, CanGetNumberOfChannelsInOutputPort) {
    GraphNode::OutputPortNodeConnections output_audio_ports{{node0, 1}};

    GraphNode node{nodes, {}, output_audio_ports};

    int expected_num_channels = 2;
    EXPECT_CALL(*node0, getAudioOutputPortChannels(1))
        .WillOnce(Return(expected_num_channels));

    ASSERT_THAT(node.getAudioOutputPortChannels(0), Eq(expected_num_channels));
}

TEST_F(AGraphNode, DefaultIdIsEmptry) {
    GraphNode node{{}, {}, {}};

    ASSERT_THAT(node.getNodeID(), Eq(""));
}

TEST_F(AGraphNode, CanSetNodeId) {
    auto node_id = "ABC";
    GraphNode node{{}, {}, {}};

    node.setNodeID(node_id);

    ASSERT_THAT(node.getNodeID(), Eq(node_id));
}

TEST_F(AGraphNode, CanPrepareEveryNode) {
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, prepareToPlay(sample_rate, max_block_size)).Times(1);
    EXPECT_CALL(*node1, prepareToPlay(sample_rate, max_block_size)).Times(1);

    node.prepareToPlay(sample_rate, max_block_size);
}

TEST_F(AGraphNode, CanPrepareForNextBlockForEveryNode) {
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, prepareForNextBlock).Times(1);
    EXPECT_CALL(*node1, prepareForNextBlock).Times(1);

    node.prepareForNextBlock();
}

TEST_F(AGraphNode, HasProcessedReturnsTrueIfAllNodesProcessed) {
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, hasProcessed).WillOnce(Return(true));
    EXPECT_CALL(*node1, hasProcessed).WillOnce(Return(true));

    ASSERT_TRUE(node.hasProcessed());
}

TEST_F(AGraphNode, HasProcessedReturnsFalseIfHasNodeNotProcessed) {
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, hasProcessed).WillOnce(Return(false));

    ASSERT_FALSE(node.hasProcessed());
}

TEST_F(AGraphNode, CanSetProcessedStateForEveryNode) {
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, setProcessedState(true)).Times(1);
    EXPECT_CALL(*node1, setProcessedState(true)).Times(1);

    node.setProcessedState(true);
}

TEST_F(AGraphNode, AddUpstreamAudioConnectionWillConnectInternalNodes) {
    GraphNode::InputPortNodeConnections input_audio_ports{
        {{node0, 0}, {node1, 1}},
    };
    GraphNode::OutputPortNodeConnections output_audio_ports{{node0, 0}};
    GraphNode node{nodes, input_audio_ports, output_audio_ports};
    AudioConnection c(upstream_node, 0, 0);

    AudioConnection expected_con_0{upstream_node, 0, 0};
    AudioConnection expected_con_1{upstream_node, 0, 1};
    EXPECT_CALL(*node0, addUpstreamAudioConnection(expected_con_0)).Times(1);
    EXPECT_CALL(*node1, addUpstreamAudioConnection(expected_con_1)).Times(1);

    node.addUpstreamAudioConnection(c);
}

TEST_F(AGraphNode, AddUpstreamParameterChangeConnectionWillConnectInternalNodes) {
    GraphNode::InputPortNodeConnections input_pc_ports{
        {{node0, 0}, {node1, 1}},
    };

    GraphNode node{nodes, {}, input_pc_ports, {}, {}};
    ParameterChangeConnection c(upstream_node, 0, 0);

    ParameterChangeConnection expected_con_0{upstream_node, 0, 0};
    ParameterChangeConnection expected_con_1{upstream_node, 0, 1};
    EXPECT_CALL(*node0, addUpstreamParameterChangeConnection(expected_con_0)).Times(1);
    EXPECT_CALL(*node1, addUpstreamParameterChangeConnection(expected_con_1)).Times(1);

    node.addUpstreamParameterChangeConnection(c);
}

TEST_F(AGraphNode, PullAudioPortWillPullUpstreamData) {
    auto proc0 = std::make_shared<NiceMock<MockProcessor>>();
    auto proc1 = std::make_shared<NiceMock<MockProcessor>>();
    auto proc_node0 = std::make_shared<ProcessorNode>(proc0);
    auto proc_node1 = std::make_shared<ProcessorNode>(proc1);

    proc_node1->addUpstreamAudioConnection(AudioConnection{proc_node0, 0, 0});

    GraphNode::OutputPortNodeConnections output_audio_ports{{proc_node1, 0}};
    GraphNode node{
        {proc_node0, proc_node1}, {}, output_audio_ports};

    EXPECT_CALL(*proc0, processBlock).Times(1);
    EXPECT_CALL(*proc1, processBlock)
        .WillOnce([this](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&this->block.buffer,
                                    this->block.buffer.getNumberChannels(),
                                    this->block.buffer.getNumberFrames(), 0, 0);
        });

    node.prepareToPlay(sample_rate, max_block_size);
    AudioPort &output_port = node.pullAudioPort(0);

    ASSERT_THAT(output_port.getChannelData(0)[0], Eq(1));
    ASSERT_THAT(output_port.getChannelData(1)[0], Eq(2));
}

TEST_F(AGraphNode, PullParameterChangePortWillPullUpstreamData) {
    auto proc0 = std::make_shared<NiceMock<MockProcessor>>();
    auto proc1 = std::make_shared<NiceMock<MockProcessor>>();
    auto proc_node0 = std::make_shared<ProcessorNode>(proc0);
    auto proc_node1 = std::make_shared<ProcessorNode>(proc1);

    proc_node1->addUpstreamParameterChangeConnection(ParameterChangeConnection{proc_node0, 0, 0});
    GraphNode::OutputPortNodeConnections output_pc_ports{{proc_node1, 0}};
    GraphNode node{
        {proc_node0, proc_node1}, {}, {}, {}, output_pc_ports};

    EXPECT_CALL(*proc0, processBlock).Times(1);
    EXPECT_CALL(*proc1, processBlock)
        .WillOnce([this](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&this->block.buffer,
                                    this->block.buffer.getNumberChannels(),
                                    this->block.buffer.getNumberFrames(), 0, 0);
            output->param_changes.push(0, {0, 1, 1});
        });

    node.prepareToPlay(sample_rate, max_block_size);
    ParameterChangePort &port = node.pullParameterChangePort(0);
    ParameterChangePoint result{};
    port.getParameterChanges().pop(0, result);

    ASSERT_THAT(port.getParameterChanges().getNumParameters(), Eq(proc0->getParameters()->size()));
    ASSERT_THAT(result.index, Eq(0));
    ASSERT_THAT(result.time, Eq(1));
    ASSERT_THAT(result.normalized_value, Eq(1));
}
