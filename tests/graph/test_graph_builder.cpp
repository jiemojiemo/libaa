//
// Created by user on 5/13/22.
//

#include "libaa/graph/aa_graph_builder.h"
#include "libaa/processor/aa_delay_processor.h"
#include "libaa_testing/aa_mock_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AGraphBuilder : public Test {
public:
    void SetUp() override {
        gain_node->setNodeID(gain_node_id);
        delay_node->setNodeID(delay_node_id);
    }

    auto makeExpectedGraphJson() {
        std::shared_ptr<GainProcessor> gain = std::make_shared<GainProcessor>();
        std::shared_ptr<DelayProcessor> delay = std::make_shared<DelayProcessor>();
        std::shared_ptr<ProcessorNode> gain_node = std::make_shared<ProcessorNode>(gain);
        std::shared_ptr<ProcessorNode> delay_node = std::make_shared<ProcessorNode>(delay);

        gain_node->setNodeID(gain_node_id);
        delay_node->setNodeID(delay_node_id);

        GraphNode::InputPortNodeConnections input_audio_ports{
            {{gain_node, 0}, {delay_node, 0}},
            {{gain_node, 0}, {delay_node, 0}}};
        GraphNode::InputPortNodeConnections input_pc_ports{
            {{gain_node, 0}, {delay_node, 0}},
            {{gain_node, 0}, {delay_node, 0}}};
        GraphNode::OutputPortNodeConnections output_audio_ports{
            {gain_node, 0}, {delay_node, 0}};
        GraphNode::OutputPortNodeConnections output_pc_ports{
            {gain_node, 0}, {delay_node, 0}};
        delay_node->addUpstreamAudioConnection(AudioConnection{gain_node, 0, 0});
        delay_node->addUpstreamParameterChangeConnection(ParameterChangeConnection{gain_node, 0, 0});

        GraphNode expected_node{{gain_node, delay_node}, input_audio_ports, input_pc_ports, output_audio_ports, output_pc_ports};
        auto expected_node_state = expected_node.getState();
        auto expected_node_state_json = JsonUtilities::binaryDataToJson(expected_node_state);

        return expected_node_state_json;
    };

    GraphBuilder builder;
    std::shared_ptr<GainProcessor> gain = std::make_shared<GainProcessor>();
    std::shared_ptr<DelayProcessor> delay = std::make_shared<DelayProcessor>();
    std::shared_ptr<ProcessorNode> gain_node = std::make_shared<ProcessorNode>(gain);
    std::shared_ptr<ProcessorNode> delay_node = std::make_shared<ProcessorNode>(delay);
    std::string gain_node_id = "gain_node";
    std::string delay_node_id = "delay_node";
};

TEST_F(AGraphBuilder, HasNoNodesWhenInit) {
    ASSERT_TRUE(builder.getNodes().empty());
}

TEST_F(AGraphBuilder, InserNodeIncreaseTheNodeSize) {
    builder.insert("node", gain_node);

    ASSERT_THAT(builder.getNodes().size(), Eq(1));
}

TEST_F(AGraphBuilder, InsertNodeWillResetNodeId) {
    auto node_id = "abc";
    builder.insert(node_id, gain_node);

    ASSERT_THAT(gain_node->getNodeID(), Eq(node_id));
}

TEST_F(AGraphBuilder, InsertNodeFailedIfNodeIdExisted) {
    builder.insert("node", gain_node);

    bool insert_ok = builder.insert("node", gain_node);
    ASSERT_FALSE(insert_ok);
    ASSERT_THAT(builder.getNodes().size(), Eq(1));
}

TEST_F(AGraphBuilder, AddAudioConnectionBuildNodeAudioConnection) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    builder.addConnection(ConnectionType::kAudioConnection, {gain_node, 0}, {delay_node, 0});

    AudioConnection expected_con{gain_node, 0, 0};
    ASSERT_THAT(delay_node->getUpstreamAudioConnections().size(), Eq(1));
    ASSERT_THAT(delay_node->getUpstreamAudioConnections()[0], Eq(expected_con));
}

TEST_F(AGraphBuilder, AddParamChangeConnectionBuildNodeParamConnection) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    builder.addConnection(ConnectionType::kParameterChangeConnection, {gain_node, 0}, {delay_node, 0});

    ParameterChangeConnection expected_con{gain_node, 0, 0};
    ASSERT_THAT(delay_node->getUpstreamParameterConnections().size(), Eq(1));
    ASSERT_THAT(delay_node->getUpstreamParameterConnections()[0], Eq(expected_con));
}

TEST_F(AGraphBuilder, AddConnectionFaildIfPortIndexInvalid) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    int invalid_up_port_index = 2;
    bool is_ok = builder.addConnection(ConnectionType::kAudioConnection, {gain_node, invalid_up_port_index}, {delay_node, 0});

    ASSERT_FALSE(is_ok);
}

TEST_F(AGraphBuilder, CanAddConnectionWithNodeId) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    bool is_ok = builder.addConnection(ConnectionType::kParameterChangeConnection, {"node0", 0}, {"node1", 0});

    ASSERT_TRUE(is_ok);
}

TEST_F(AGraphBuilder, AddConnectionThrowsIfCannotFindUpstreamNode) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    ASSERT_ANY_THROW(builder.addConnection(ConnectionType::kParameterChangeConnection, {"not_exist_node", 0}, {"node1", 0}));
}

TEST_F(AGraphBuilder, AddConnectionThrowsIfCannotFindDownstreamNode) {
    builder.insert("node0", gain_node);
    builder.insert("node1", delay_node);

    ASSERT_ANY_THROW(builder.addConnection(ConnectionType::kParameterChangeConnection, {"node0", 0}, {"not_exist_node", 0}));
}

TEST_F(AGraphBuilder, HasNoInputAudioPortWhenInit) {
    ASSERT_TRUE(builder.getExposedInputAudioPorts().empty());
}

TEST_F(AGraphBuilder, ExposeInputAudioPortIncreaseInputAudioPortSize) {
    builder.insert("node0", gain_node);

    builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {gain_node, 0});

    ASSERT_THAT(builder.getExposedInputAudioPorts().size(), Eq(1));
}

TEST_F(AGraphBuilder, ExposeInputParameChangePortIncreaseParamChangePortSize) {
    builder.insert("node0", gain_node);

    builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 0, {gain_node, 0});

    ASSERT_THAT(builder.getExposedInputParameterChangePorts().size(), Eq(1));
}

TEST_F(AGraphBuilder, ExposeOutputAudioPortIncreaseInputAudioPortSize) {
    builder.insert("node0", gain_node);

    builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {gain_node, 0});

    ASSERT_THAT(builder.getExposedOutputAudioPorts().size(), Eq(1));
}

TEST_F(AGraphBuilder, ExposeOutputParameChangePortIncreaseParamChangePortSize) {
    builder.insert("node0", gain_node);

    builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, 0, {gain_node, 0});

    ASSERT_THAT(builder.getExposedOutputParameterChangePorts().size(), Eq(1));
}

TEST_F(AGraphBuilder, ExposeInputAudioPortThrowsIfNodePortIndexInvalid) {
    builder.insert("node0", gain_node);

    int invalid_node_port = 2;

    ASSERT_THAT(invalid_node_port, Ge(gain_node->getAudioInputPortSize()));
    ASSERT_ANY_THROW(builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {gain_node, invalid_node_port}));
}

TEST_F(AGraphBuilder, ExposeInputParameterChangePortThrowsIfNodePortIndexInvalid) {
    builder.insert("node0", gain_node);

    int invalid_node_port = 2;

    ASSERT_THAT(invalid_node_port, Ge(gain_node->getAudioInputPortSize()));
    ASSERT_ANY_THROW(builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 0, {gain_node, invalid_node_port}));
}

TEST_F(AGraphBuilder, ExposeOutputAudioPortThrowsIfNodePortIndexInvalid) {
    builder.insert("node0", gain_node);

    int invalid_node_port = 2;

    ASSERT_THAT(invalid_node_port, Ge(gain_node->getAudioInputPortSize()));
    ASSERT_ANY_THROW(builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {gain_node, invalid_node_port}));
}

TEST_F(AGraphBuilder, ExposeOutputParameterChangePortThrowsIfNodePortIndexInvalid) {
    builder.insert("node0", gain_node);

    int invalid_node_port = 2;

    ASSERT_THAT(invalid_node_port, Ge(gain_node->getAudioInputPortSize()));
    ASSERT_ANY_THROW(builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, 0, {gain_node, invalid_node_port}));
}

TEST_F(AGraphBuilder, CanExposePortWithNodeId) {
    builder.insert("node0", gain_node);

    builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {"node0", 0});

    ASSERT_THAT(builder.getExposedInputAudioPorts().size(), Eq(1));
}

TEST_F(AGraphBuilder, ExposePortWithNodeIdThrowsIfNotSuchNode) {
    builder.insert("node0", gain_node);

    ASSERT_ANY_THROW(builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {"not_exist_node", 0}));
}

TEST_F(AGraphBuilder, CanBuildNodeExpected) {
    auto expected_node_state_json = makeExpectedGraphJson();

    builder.insert(gain_node_id, gain_node);
    builder.insert(delay_node_id, delay_node);
    builder.addConnection(ConnectionType::kAudioConnection, {gain_node, 0}, {delay_node, 0});
    builder.addConnection(ConnectionType::kParameterChangeConnection, {gain_node, 0}, {delay_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {gain_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {delay_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kAudio, 1, {gain_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kAudio, 1, {delay_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 0, {gain_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 0, {delay_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 1, {gain_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 1, {delay_node, 0});
    builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {gain_node, 0});
    builder.exposePort(PortDirection::kOutput, PortType::kAudio, 1, {delay_node, 0});
    builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, 0, {gain_node, 0});
    builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, 1, {delay_node, 0});

    auto graph = builder.build();
    auto graph_state_json = JsonUtilities::binaryDataToJson(graph->getState());
    ASSERT_THAT(graph_state_json, Eq(expected_node_state_json));
}