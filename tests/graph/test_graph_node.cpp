//
// Created by user on 5/7/22.
//
#include "libaa/aa_version.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_graph_node.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/processor/aa_delay_processor.h"
#include "libaa/processor/aa_gain_processor.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa/processor/aa_source_processor.h"
#include "libaa_testing/aa_mock_node.h"
#include "libaa_testing/aa_mock_processor.h"
#include "libaa/graph/aa_transport_context.h"

#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;
class AGraphNode : public Test {
public:
    void SetUp() override {
        node0->setNodeID("node0");
        node1->setNodeID("node1");
        gain_node->setNodeID("gain_node");
        delay_node->setNodeID("delay_node");
    }

    std::shared_ptr<MockNode> node0 = std::make_shared<MockNode>();
    std::shared_ptr<MockNode> node1 = std::make_shared<MockNode>();
    std::shared_ptr<MockNode> upstream_node = std::make_shared<MockNode>();
    std::vector<std::shared_ptr<INode>> nodes{node0, node1};
    std::shared_ptr<GainProcessor> gain = std::make_shared<GainProcessor>();
    std::shared_ptr<DelayProcessor> delay = std::make_shared<DelayProcessor>();
    std::shared_ptr<ProcessorNode> gain_node = std::make_shared<ProcessorNode>(gain);
    std::shared_ptr<ProcessorNode> delay_node = std::make_shared<ProcessorNode>(delay);
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

TEST_F(AGraphNode, CanSetTransportContextForEveryNode) {
    auto transport_context = std::make_shared<TransportContext>();
    GraphNode node{nodes, {}, {}};

    EXPECT_CALL(*node0, setTransportContext).Times(1);
    EXPECT_CALL(*node1, setTransportContext).Times(1);

    node.setTransportContext(transport_context);
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

TEST_F(AGraphNode, AddUpstreamAudioConnectionIncreasesTheConnectionSize) {
    GraphNode::InputPortNodeConnections input_audio_ports{
        {{node0, 0}, {node1, 1}},
    };
    GraphNode node{nodes, input_audio_ports, {}};
    AudioConnection c(upstream_node, 0, 0);

    node.addUpstreamAudioConnection(c);

    ASSERT_THAT(node.getUpstreamAudioConnections().size(), Eq(1));
}

TEST_F(AGraphNode, AddUpstreamParameterChangeConnectionIncreasesTheConnectionSize) {
    GraphNode::InputPortNodeConnections input_pc_ports{
        {{node0, 0}, {node1, 1}},
    };

    GraphNode node{nodes, {}, input_pc_ports, {}, {}};
    ParameterChangeConnection c(upstream_node, 0, 0);

    node.addUpstreamParameterChangeConnection(c);

    ASSERT_THAT(node.getUpstreamParameterConnections().size(), Eq(1));
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

TEST_F(AGraphNode, NodeStateContainsNodeId) {
    auto node_id = "ABC";
    GraphNode node{{}, {}, {}};
    node.setNodeID(node_id);

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    ASSERT_FALSE(node_json["node_id"].is_null());
    ASSERT_THAT(node_json["node_id"].get<std::string>(), Eq(node_id));
}

TEST_F(AGraphNode, NodeStateContainsVersion) {
    GraphNode node{{}, {}, {}};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    ASSERT_FALSE(node_json["version"].is_null());
    ASSERT_THAT(node_json["version"].get<std::string>(), Eq(LIBAA_VERSION));
}

TEST_F(AGraphNode, NodeStateContainsNodeTypeString) {
    GraphNode node{{}, {}, {}};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    ASSERT_FALSE(node_json["node_type"].is_null());
    ASSERT_THAT(node_json["node_type"].get<std::string>(), Eq("graph_node"));
}

TEST_F(AGraphNode, NodeStateContainsInputAudioPorts) {
    GraphNode::InputPortNodeConnections input_audio_ports{
        {{node0, 0}, {node1, 0}},
        {{node0, 1}, {node1, 1}}};
    GraphNode node{nodes, input_audio_ports, {}};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "ports":
        [
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 0,
                "port_type": "audio",
                "port_direction": "input",
                "port_index": 0
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 0,
                "port_type": "audio",
                "port_direction": "input",
                "port_index": 0
            },
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 1,
                "port_type": "audio",
                "port_direction": "input",
                "port_index": 1
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 1,
                "port_type": "audio",
                "port_direction": "input",
                "port_index": 1
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["ports"].is_array());
    ASSERT_THAT(node_json["ports"], Eq(expected_json["ports"]));
}

TEST_F(AGraphNode, NodeStateContainsOutputAudioPorts) {
    GraphNode::OutputPortNodeConnections output_audio_ports{
        {node0, 0},
        {node1, 1}};

    GraphNode node{nodes, {}, output_audio_ports};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "ports":
        [
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 0,
                "port_type": "audio",
                "port_direction": "output",
                "port_index": 0
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 1,
                "port_type": "audio",
                "port_direction": "output",
                "port_index": 1
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["ports"].is_array());
    ASSERT_THAT(node_json["ports"], Eq(expected_json["ports"]));
}

TEST_F(AGraphNode, NodeStateContainsInputParameterChangePorts) {
    GraphNode::InputPortNodeConnections input_pc_ports{
        {{node0, 0}, {node1, 0}},
        {{node0, 1}, {node1, 1}}};

    GraphNode node{nodes, {}, input_pc_ports, {}, {}};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "ports":
        [
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 0,
                "port_type": "parameter_change",
                "port_direction": "input",
                "port_index": 0
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 0,
                "port_type": "parameter_change",
                "port_direction": "input",
                "port_index": 0
            },
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 1,
                "port_type": "parameter_change",
                "port_direction": "input",
                "port_index": 1
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 1,
                "port_type": "parameter_change",
                "port_direction": "input",
                "port_index": 1
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["ports"].is_array());
    ASSERT_THAT(node_json["ports"], Eq(expected_json["ports"]));
}

TEST_F(AGraphNode, NodeStateContainsOutputParameterChangePorts) {
    GraphNode::OutputPortNodeConnections output_pc_ports{
        {node0, 0},
        {node1, 1}};

    GraphNode node{nodes, {}, {}, {}, output_pc_ports};

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "ports":
        [
            {
                "internal_node_id" : "node0",
                "internal_node_port_index": 0,
                "port_type": "parameter_change",
                "port_direction": "output",
                "port_index": 0
            },
            {
                "internal_node_id" : "node1",
                "internal_node_port_index": 1,
                "port_type": "parameter_change",
                "port_direction": "output",
                "port_index": 1
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["ports"].is_array());
    ASSERT_THAT(node_json["ports"], Eq(expected_json["ports"]));
}

TEST_F(AGraphNode, NodeStateContainsInternalNodeAudioConnections) {
    delay_node->addUpstreamAudioConnection(AudioConnection{gain_node, 0, 0});

    GraphNode node{
        {gain_node, delay_node}, {}, {}};
    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "connections":
        [
            {
                "downstream_node_id" : "delay_node",
                "downstream_node_port": 0,
                "upstream_node_id": "gain_node",
                "upstream_node_port": 0,
                "port_type": "audio"
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["connections"].is_array());
    ASSERT_THAT(node_json["connections"], Eq(expected_json["connections"]));
}

TEST_F(AGraphNode, NodeStateContainsInternalNodeParameterChangeConnections) {
    delay_node->addUpstreamParameterChangeConnection(ParameterChangeConnection{gain_node, 0, 0});

    GraphNode node{
        {gain_node, delay_node}, {}, {}};
    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
    {
        "connections":
        [
            {
                "downstream_node_id" : "delay_node",
                "downstream_node_port": 0,
                "upstream_node_id": "gain_node",
                "upstream_node_port": 0,
                "port_type": "parameter_change"
            }
        ]
    }
    )");

    ASSERT_TRUE(node_json["connections"].is_array());
    ASSERT_THAT(node_json["connections"], Eq(expected_json["connections"]));
}

TEST_F(AGraphNode, NodeStateContainsInternalNodesState) {
    auto gain_node = std::make_shared<ProcessorNode>(gain);
    auto delay_node = std::make_shared<ProcessorNode>(delay);
    gain_node->setNodeID("gain");
    delay_node->setNodeID("delay");
    delay_node->addUpstreamAudioConnection(AudioConnection{gain_node, 0, 0});

    GraphNode::OutputPortNodeConnections output_audio_ports{{delay_node, 0}};
    GraphNode node{
        {gain_node, delay_node}, {}, {}};

    nlohmann::json expected_json;
    auto gain_node_state_json = NodeSerializationUtilities::binaryDataToJson(gain_node->getState());
    auto delay_node_state_json = NodeSerializationUtilities::binaryDataToJson(delay_node->getState());
    expected_json["nodes"].push_back(gain_node_state_json);
    expected_json["nodes"].push_back(delay_node_state_json);

    auto node_json = NodeSerializationUtilities::binaryDataToJson(node.getState());

    ASSERT_TRUE(node_json["nodes"].is_array());
    ASSERT_THAT(node_json["nodes"], Eq(expected_json["nodes"]));
}

TEST_F(AGraphNode, SetStateCanReCreatesInternalProcessorNodes) {
    auto gain_node = std::make_shared<ProcessorNode>(gain);
    auto delay_node = std::make_shared<ProcessorNode>(delay);
    gain_node->setNodeID("gain");
    delay_node->setNodeID("delay");
    delay_node->addUpstreamAudioConnection(AudioConnection{gain_node, 0, 0});
    auto processors_node = std::shared_ptr<GraphNode>(new GraphNode{{gain_node, delay_node}, {}, {}});
    GraphNode graph_node{
        {processors_node, gain_node, delay_node}, {}, {}};

    GraphNode node{{}, {}, {}};

    auto other_node_state = graph_node.getState();
    node.setState(other_node_state.data(), other_node_state.size());

    ASSERT_THAT(node.getAllNodes().size(), Eq(3));
}

TEST_F(AGraphNode, SetStateThrowsIfNodeTypeUnsupported) {
    auto gain_node = std::make_shared<ProcessorNode>(gain);
    gain_node->setNodeID("gain");
    GraphNode graph_node{
        {gain_node}, {}, {}};

    auto other_node_state_json = NodeSerializationUtilities::binaryDataToJson(graph_node.getState());
    other_node_state_json["nodes"][0]["node_type"] = "unknown";
    auto other_node_state = NodeSerializationUtilities::jsonToBinaryData(other_node_state_json);

    GraphNode node{{}, {}, {}};

    ASSERT_ANY_THROW(node.setState(other_node_state.data(), other_node_state.size()));
}

TEST_F(AGraphNode, SetStateUpdatesNodeId) {
    auto node_id = "abc";
    GraphNode other_node{{}, {}, {}};
    other_node.setNodeID(node_id);

    auto other_node_state = other_node.getState();

    GraphNode node{{}, {}, {}};
    node.setState(other_node_state.data(), other_node_state.size());

    ASSERT_THAT(node.getNodeID(), Eq(node_id));
}

TEST_F(AGraphNode, SetStateRebuildInputAudioPorts) {
    GraphNode::InputPortNodeConnections input_audio_ports{
        {{gain_node, 0}, {delay_node, 0}},
        {{gain_node, 1}, {delay_node, 1}}};

    GraphNode expected_node{{gain_node, delay_node}, input_audio_ports, {}};
    auto expected_node_state = expected_node.getState();
    auto expected_node_state_json = NodeSerializationUtilities::binaryDataToJson(expected_node_state);

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(input_audio_ports.size()));
    ASSERT_THAT(node.getInputAudioPortConnections()[0].size(), Eq(2));
    ASSERT_THAT(node.getInputAudioPortConnections()[0].front().node_port_index, Eq(0));
    ASSERT_THAT(node.getInputAudioPortConnections()[0].back().node_port_index, Eq(0));
    ASSERT_THAT(node.getInputAudioPortConnections()[1].size(), Eq(2));
    ASSERT_THAT(node.getInputAudioPortConnections()[1].front().node_port_index, Eq(1));
    ASSERT_THAT(node.getInputAudioPortConnections()[1].back().node_port_index, Eq(1));
}

TEST_F(AGraphNode, SetStateRebuildOutputAudioPorts) {
    GraphNode::OutputPortNodeConnections output_audio_ports{{gain_node, 0}, {delay_node, 1}};

    GraphNode expected_node{{gain_node, delay_node}, {}, output_audio_ports};
    auto expected_node_state = expected_node.getState();
    auto expected_node_state_json = NodeSerializationUtilities::binaryDataToJson(expected_node_state);

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(output_audio_ports.size()));
    ASSERT_THAT(node.getOutputAudioPortConnections().size(), Eq(output_audio_ports.size()));
    ASSERT_THAT(node.getOutputAudioPortConnections()[0].node->getNodeID(), Eq(gain_node->getNodeID()));
    ASSERT_THAT(node.getOutputAudioPortConnections()[0].node_port_index, Eq(0));
    ASSERT_THAT(node.getOutputAudioPortConnections()[1].node->getNodeID(), Eq(delay_node->getNodeID()));
    ASSERT_THAT(node.getOutputAudioPortConnections()[1].node_port_index, Eq(1));
}

TEST_F(AGraphNode, SetStateRebuildInputParameterChangePorts) {
    GraphNode::InputPortNodeConnections input_pc_ports{
        {{gain_node, 0}, {delay_node, 0}},
        {{gain_node, 1}, {delay_node, 1}}};

    GraphNode expected_node{{gain_node, delay_node}, {}, input_pc_ports, {}, {}};
    auto expected_node_state = expected_node.getState();
    auto expected_node_state_json = NodeSerializationUtilities::binaryDataToJson(expected_node_state);

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    ASSERT_THAT(node.getParameterChangeInputPortSize(), Eq(input_pc_ports.size()));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[0].size(), Eq(2));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[0].front().node_port_index, Eq(0));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[0].back().node_port_index, Eq(0));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[1].size(), Eq(2));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[1].front().node_port_index, Eq(1));
    ASSERT_THAT(node.getInputParameterChangePortConnections()[1].back().node_port_index, Eq(1));
}

TEST_F(AGraphNode, SetStateRebuildOutputParameterChangePorts) {
    GraphNode::OutputPortNodeConnections output_pc_ports{{gain_node, 0}, {delay_node, 1}};

    GraphNode expected_node{{gain_node, delay_node}, {}, {}, {}, output_pc_ports};
    auto expected_node_state = expected_node.getState();
    auto expected_node_state_json = NodeSerializationUtilities::binaryDataToJson(expected_node_state);

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    ASSERT_THAT(node.getParameterChangeOutputPortSize(), Eq(output_pc_ports.size()));
    ASSERT_THAT(node.getOutputParameterChangePortConnections().size(), Eq(output_pc_ports.size()));
    ASSERT_THAT(node.getOutputParameterChangePortConnections()[0].node->getNodeID(), Eq(gain_node->getNodeID()));
    ASSERT_THAT(node.getOutputParameterChangePortConnections()[0].node_port_index, Eq(0));
    ASSERT_THAT(node.getOutputParameterChangePortConnections()[1].node->getNodeID(), Eq(delay_node->getNodeID()));
    ASSERT_THAT(node.getOutputParameterChangePortConnections()[1].node_port_index, Eq(1));
}

TEST_F(AGraphNode, SetStateRebuildNodeAudioConnections) {
    delay_node->addUpstreamAudioConnection(AudioConnection{gain_node, 0, 0});
    GraphNode expected_node{
        {gain_node, delay_node}, {}, {}};
    auto expected_node_state = expected_node.getState();
    auto expected_json = nlohmann::json::parse(R"(
    {
        "connections":
        [
            {
                "downstream_node_id" : "delay_node",
                "downstream_node_port": 0,
                "upstream_node_id": "gain_node",
                "upstream_node_port": 0,
                "port_type": "audio"
            }
        ]
    }
    )");

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    auto node_state_json = NodeSerializationUtilities::binaryDataToJson(node.getState());
    ASSERT_THAT(node_state_json["connections"], Eq(expected_json["connections"]));
}

TEST_F(AGraphNode, SetStateRebuildNodeParameterChangeConnections) {
    delay_node->addUpstreamParameterChangeConnection(ParameterChangeConnection{gain_node, 0, 0});
    GraphNode expected_node{
        {gain_node, delay_node}, {}, {}};
    auto expected_node_state = expected_node.getState();
    auto expected_json = nlohmann::json::parse(R"(
    {
        "connections":
        [
            {
                "downstream_node_id" : "delay_node",
                "downstream_node_port": 0,
                "upstream_node_id": "gain_node",
                "upstream_node_port": 0,
                "port_type": "parameter_change"
            }
        ]
    }
    )");

    GraphNode node{{}, {}, {}};
    node.setState(expected_node_state.data(), expected_node_state.size());

    auto node_state_json = NodeSerializationUtilities::binaryDataToJson(node.getState());
    ASSERT_THAT(node_state_json["connections"], Eq(expected_json["connections"]));
}