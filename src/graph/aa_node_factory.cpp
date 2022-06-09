//
// Created by user on 6/9/22.
//

#include "libaa/graph/aa_node_factory.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_graph_builder.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_processor_factory.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
namespace libaa {
namespace NodeFactory {

namespace {
std::unique_ptr<INode> buildNode(const nlohmann::json &state_json,
                                 const ResourcePathFinder *finder);

std::string fileToString(const std::string &file_path) {
    std::ifstream in_file(file_path);
    if (!in_file.is_open()) {
        throw std::invalid_argument("cannot open " + file_path);
    }

    std::stringstream buffer;
    buffer << in_file.rdbuf();
    return buffer.str();
}

auto buildProcessorNode(const nlohmann::json &state_json,
                        const ResourcePathFinder *finder) {
    const nlohmann::json &proc_state = state_json["processor_state"];
    auto proc_name = proc_state["processor_name"].get<std::string>();
    std::shared_ptr<IAudioProcessor> proc = ProcessorFactory::create(proc_name, nlohmann::to_string(proc_state), finder);
    auto input_channels = state_json["input_channels"].get<std::vector<int>>();
    auto output_channels = state_json["output_channels"].get<std::vector<int>>();
    auto node = std::make_unique<ProcessorNode>(proc, input_channels, output_channels);
    auto node_id = state_json["node_id"].get<std::string>();
    node->setNodeID(node_id);
    return node;
}

auto buildGraphNode(const nlohmann::json &state_json,
                    const ResourcePathFinder *finder) {
    GraphBuilder builder;
    // build nodes
    for (const auto &n : state_json["nodes"]) {
        auto node_id = n["node_id"].get<std::string>();
        std::shared_ptr<INode> node = buildNode(n, finder);
        builder.insert(node_id, node);
    }

    // build connections
    for (const auto &connection_json : state_json["connections"]) {
        auto downstream_node_id = connection_json["downstream_node_id"].get<std::string>();
        auto downstream_node_port = connection_json["downstream_node_port"].get<int>();
        auto upstream_node_id = connection_json["upstream_node_id"].get<std::string>();
        auto upstream_node_port = connection_json["upstream_node_port"].get<int>();
        auto port_type_str = connection_json["port_type"].get<std::string>();
        auto port_type = NodeUtilities::getConnectionTypeFromString(port_type_str);
        builder.addConnection(port_type,
                              {upstream_node_id, upstream_node_port},
                              {downstream_node_id, downstream_node_port});
    }

    // build ports
    for (const auto &p : state_json["ports"]) {
        auto internal_node_id = p["internal_node_id"].get<std::string>();
        auto internal_node_port_index = p["internal_node_port_index"].get<int>();
        auto port_direction_str = p["port_direction"].get<std::string>();
        auto port_direction = NodeUtilities::getPortDirectionTypeFromString(port_direction_str);
        auto port_index = p["port_index"].get<int>();
        auto port_type_str = p["port_type"].get<std::string>();
        auto port_type = NodeUtilities::getPortTypeFromString(port_type_str);

        builder.exposePort(port_direction, port_type, port_index, {internal_node_id, internal_node_port_index});
    }

    auto node = builder.build();
    auto node_id = state_json["node_id"].get<std::string>();
    node->setNodeID(node_id);
    return node;
}

std::unique_ptr<INode> buildNode(const nlohmann::json &state_json,
                                 const ResourcePathFinder *finder) {
    auto node_type = state_json["node_type"].get<std::string>();
    if (node_type == "processor_node") {
        return buildProcessorNode(state_json, finder);
    } else if (node_type == "graph_node") {
        return buildGraphNode(state_json, finder);
    } else {
        throw std::invalid_argument(node_type + " is unsupported node type");
    }
}

} // namespace

std::unique_ptr<INode> build(uint8_t *state,
                             size_t size,
                             const ResourcePathFinder *finder) {
    auto state_json = JsonUtilities::binaryDataToJson(state, size);
    return buildNode(state_json, finder);
}

std::unique_ptr<INode> buildFromFilePath(const std::string &file_path,
                                         const ResourcePathFinder *finder) {
    // file to string
    auto file_str = fileToString(file_path);
    return build((uint8_t *)file_str.data(), file_str.size(), finder);
}

} // namespace NodeFactory
} // namespace libaa
