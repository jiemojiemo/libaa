//
// Created by user on 5/26/22.
//

#include "../aa_testing_helper.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/graph/aa_graph_builder.h"
#include "libaa/graph/aa_node_factory.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/processor/aa_channel_vocoder_processor.h"
#include "libaa/processor/aa_processor_factory.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace std;
using namespace libaa;
class ANodeFactory : public Test {
public:
    auto buildProcessorNode(const std::string &node_id) {
        shared_ptr<IAudioProcessor> proc = ProcessorFactory::create("Gain");
        auto node = make_shared<ProcessorNode>(proc);
        node->setNodeID(node_id);
        return node;
    }

    auto buildGraphNode(const std::string &node_id) {
        shared_ptr<IAudioProcessor> proc0 = ProcessorFactory::create("Gain");
        shared_ptr<IAudioProcessor> proc1 = ProcessorFactory::create("Gain");
        auto node0 = std::make_shared<ProcessorNode>(proc0);
        auto node1 = std::make_shared<ProcessorNode>(proc0);

        GraphBuilder builder;
        builder.insert("gain0", node0);
        builder.insert("gain1", node1);
        builder.addConnection(ConnectionType::kAudioConnection, {node0, 0}, {node1, 0});
        builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {node0, 0});
        builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {node1, 0});
        auto node = builder.build();
        node->setNodeID(node_id);
        return node;
    }

    auto buildChannelProcessorNode(
        const std::string &carrier_file_uri,
        ResourcePathFinder *finder) {

        std::shared_ptr<Clip> carrier_clip = ClipFactory::buildClip(carrier_file_uri, true, finder);

        auto proc = std::make_shared<ChannelVocoderProcessor>(carrier_clip, std::make_shared<ResourcePathFinder>(*finder));
        return std::make_shared<ProcessorNode>(proc);
    }

    auto buildChannelProcessorGraphNode(const std::string &carrier_file_uri,
                                        ResourcePathFinder *finder) {

        shared_ptr<IAudioProcessor> proc0 = ProcessorFactory::create("Gain");
        auto node0 = std::make_shared<ProcessorNode>(proc0);
        auto channel_vocoder_node = buildChannelProcessorNode(carrier_file_uri, finder);

        GraphBuilder builder;
        builder.insert("gain0", node0);
        builder.insert("vocoder", channel_vocoder_node);
        builder.addConnection(ConnectionType::kAudioConnection, {node0, 0}, {channel_vocoder_node, 0});
        builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {node0, 0});
        builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {channel_vocoder_node, 0});

        return builder.build();
    }
};

TEST_F(ANodeFactory, CanBuildProcessorNode) {
    auto node_id = "mock_node";
    auto expected_node = buildProcessorNode(node_id);
    auto expected_state = expected_node->getState();
    auto expected_json = JsonUtilities::binaryDataToJson(expected_state);

    auto node = NodeFactory::build(expected_state.data(), expected_state.size());
    auto node_json = JsonUtilities::binaryDataToJson(node->getState());

    ASSERT_THAT(node_json, Eq(expected_json));
}

TEST_F(ANodeFactory, CanBuildGraphNode) {
    auto expected_node = buildGraphNode("mock_node");
    auto expected_state = expected_node->getState();
    auto expected_json = JsonUtilities::binaryDataToJson(expected_state);

    auto node = NodeFactory::build(expected_state.data(), expected_state.size());
    auto node_json = JsonUtilities::binaryDataToJson(node->getState());

    ASSERT_THAT(node_json, Eq(expected_json));
}

TEST_F(ANodeFactory, ThrowsIfNodeTypeInvalid) {
    auto mock_node = buildProcessorNode("mock_node");
    auto input_json = JsonUtilities::binaryDataToJson(mock_node->getState());
    input_json["node_type"] = "invalid_type";
    auto input_state = JsonUtilities::jsonToBinaryData(input_json);

    ASSERT_ANY_THROW(NodeFactory::build(input_state.data(), input_state.size()));
}

TEST_F(ANodeFactory, CanBuildFromFilePath) {
    auto expected_node = buildGraphNode("mock_node");
    auto expected_state = expected_node->getState();
    auto expected_json = JsonUtilities::binaryDataToJson(expected_state);
    auto json_path = "scope_json.json";
    ScopeTextFile scope_json_file(json_path, to_string(expected_json));

    auto node = NodeFactory::buildFromFilePath(json_path);
    auto node_json = JsonUtilities::binaryDataToJson(node->getState());

    ASSERT_THAT(node_json, Eq(expected_json));
}

TEST_F(ANodeFactory, BuildFromFilePathThrowsIfFilePathInvalid) {
    auto invalid_path = "abc.json";

    ASSERT_ANY_THROW(NodeFactory::buildFromFilePath(invalid_path));
}

TEST_F(ANodeFactory, CanBuildProcessorNodeWithFinder) {
    auto mock_file_uri = "node_factory_mock_file";
    auto scope_file = makeScopeWavFile(mock_file_uri);
    ResourcePathFinder finder;
    auto expected_node = buildChannelProcessorNode(mock_file_uri, &finder);
    auto expected_state = expected_node->getState();
    auto expected_json = JsonUtilities::binaryDataToJson(expected_state);

    auto node = NodeFactory::build(expected_state.data(), expected_state.size(), &finder);
    auto node_json = JsonUtilities::binaryDataToJson(node->getState());

    ASSERT_THAT(node_json, Eq(expected_json));
}

TEST_F(ANodeFactory, CanBuildGraphNodeWithFinder) {
    auto mock_file_uri = "node_factory_mock_file";
    auto scope_file = makeScopeWavFile(mock_file_uri);
    ResourcePathFinder finder;
    auto expected_node = buildChannelProcessorGraphNode(mock_file_uri, &finder);
    auto expected_state = expected_node->getState();
    auto expected_json = JsonUtilities::binaryDataToJson(expected_state);

    auto node = NodeFactory::build(expected_state.data(), expected_state.size(), &finder);
    auto node_json = JsonUtilities::binaryDataToJson(node->getState());

    ASSERT_THAT(node_json, Eq(expected_json));
}