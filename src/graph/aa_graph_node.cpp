//
// Created by user on 5/7/22.
//
#include "libaa/graph/aa_graph_node.h"
#include "libaa/aa_version.h"
#include "libaa/graph/aa_parameter_change_connection.h"
#include "libaa/graph/aa_port.h"
#include <nlohmann/json.hpp>

namespace libaa {
auto serializePortConnectionToJson(const GraphNode::PortNodeConnection &port_con,
                                   PortDirection direction,
                                   PortType port_type,
                                   int graph_port_index) {
    auto type_to_string = [](PortType port_type) {
        if (port_type == PortType::kAudio) {
            return "audio";
        } else {
            return "parameter_change";
        }
    };

    auto direction_to_string = [](PortDirection direction) {
        if (direction == PortDirection::kInput) {
            return "input";
        } else {
            return "output";
        }
    };

    nlohmann::json port_json;
    port_json["internal_node_id"] = port_con.node->getNodeID();
    port_json["internal_node_port_index"] = port_con.node_port_index;
    port_json["port_type"] = type_to_string(port_type);
    port_json["port_direction"] = direction_to_string(direction);
    port_json["port_index"] = graph_port_index;

    return port_json;
}

auto serializePortsConnectionToJson(const GraphNode::InputPortNodeConnections &input_audio_port_connections_,
                                    const GraphNode::InputPortNodeConnections &input_param_change_port_connections_,
                                    const GraphNode::OutputPortNodeConnections &output_audio_port_connections_,
                                    const GraphNode::OutputPortNodeConnections &output_param_change_port_connections_) {
    nlohmann::json ports_json;

    // input audio port connection
    int graph_audio_input_port_index = 0;
    for (const auto &input_audio_ports : input_audio_port_connections_) {
        for (const auto &input_audio_port : input_audio_ports) {
            ports_json.push_back(serializePortConnectionToJson(input_audio_port,
                                                               PortDirection::kInput,
                                                               PortType::kAudio,
                                                               graph_audio_input_port_index));
        }
        ++graph_audio_input_port_index;
    }

    // input parameter change connection
    int graph_pc_input_port_index = 0;
    for (const auto &ports : input_param_change_port_connections_) {
        for (const auto &input_audio_port : ports) {
            ports_json.push_back(serializePortConnectionToJson(input_audio_port,
                                                               PortDirection::kInput,
                                                               PortType::kParameterChange,
                                                               graph_pc_input_port_index));
        }
        ++graph_pc_input_port_index;
    }

    // output audio port connection
    int graph_audio_output_port_index = 0;
    for (const auto &output_audio_port : output_audio_port_connections_) {
        ports_json.push_back(serializePortConnectionToJson(output_audio_port,
                                                           PortDirection::kOutput,
                                                           PortType::kAudio,
                                                           graph_audio_output_port_index));
        ++graph_audio_output_port_index;
    }

    // output parameter change port connection
    int graph_pc_output_port_index = 0;
    for (const auto &output_audio_port : output_param_change_port_connections_) {
        ports_json.push_back(serializePortConnectionToJson(output_audio_port,
                                                           PortDirection::kOutput,
                                                           PortType::kParameterChange,
                                                           graph_pc_output_port_index));
        ++graph_pc_output_port_index;
    }

    return ports_json;
}

GraphNode::GraphNode(std::vector<std::shared_ptr<INode>> nodes,
                     InputPortNodeConnections input_audio_port_connections,
                     OutputPortNodeConnections output_audio_port_connections)
    : nodes_(std::move(nodes)),
      input_audio_port_connections_(std::move(input_audio_port_connections)),
      output_audio_port_connections_(std::move(output_audio_port_connections)) {
}

GraphNode::GraphNode(std::vector<std::shared_ptr<INode>> nodes,
                     InputPortNodeConnections input_audio_port_connections,
                     InputPortNodeConnections input_param_change_port_connections,
                     OutputPortNodeConnections output_audio_port_connections,
                     OutputPortNodeConnections output_param_change_port_connections)
    : nodes_(std::move(nodes)),
      input_audio_port_connections_(std::move(input_audio_port_connections)),
      input_param_change_port_connections_(std::move(input_param_change_port_connections)),
      output_audio_port_connections_(std::move(output_audio_port_connections)),
      output_param_change_port_connections_(std::move(output_param_change_port_connections))

{
}

void GraphNode::prepareToPlay(float sample_rate, int max_block_size) {
    for (auto &n : nodes_) {
        n->prepareToPlay(sample_rate, max_block_size);
    }
}
void GraphNode::addUpstreamAudioConnection(const AudioConnection &c) {
    auto &downstream_audio_port_connections =
        input_audio_port_connections_.at(c.downstream_port_index);

    for (auto &port_connection : downstream_audio_port_connections) {
        auto internal_node_port_index = port_connection.node_port_index;
        AudioConnection new_audio_con{c.upstream_node, c.upstream_port_index,
                                      internal_node_port_index};

        port_connection.node->addUpstreamAudioConnection(new_audio_con);
    }

    audio_connections_.push_back(c);
}

void GraphNode::addUpstreamParameterChangeConnection(const ParameterChangeConnection &c) {
    auto &downstream_pc_port_connections =
        input_param_change_port_connections_.at(c.downstream_port_index);

    for (auto &port_connection : downstream_pc_port_connections) {
        auto internal_node_pc_port_index = port_connection.node_port_index;
        ParameterChangeConnection new_pc_con{c.upstream_node, c.upstream_port_index, internal_node_pc_port_index};

        port_connection.node->addUpstreamParameterChangeConnection(new_pc_con);
    }

    param_change_connections_.push_back(c);
}

AudioPort &GraphNode::pullAudioPort(int output_audio_port) {
    const auto &output_audio_port_connection = output_audio_port_connections_.at(output_audio_port);
    const int node_port = output_audio_port_connection.node_port_index;
    return output_audio_port_connection.node->pullAudioPort(node_port);
}

ParameterChangePort &GraphNode::pullParameterChangePort(int output_pc_port) {
    const auto &output_pc_port_connection = output_param_change_port_connections_.at(output_pc_port);
    const int node_port = output_pc_port_connection.node_port_index;
    return output_pc_port_connection.node->pullParameterChangePort(node_port);
}

bool GraphNode::hasProcessed() const {
    for (const auto &n : nodes_) {
        if (!n->hasProcessed())
            return false;
    }
    return true;
}

void GraphNode::setProcessedState(bool is_processed) {
    for (auto &n : nodes_) {
        n->setProcessedState(is_processed);
    }
}

void GraphNode::prepareForNextBlock() {
    for (auto &n : nodes_) {
        n->prepareForNextBlock();
    }
}

const std::vector<std::shared_ptr<INode>> &GraphNode::getAllNodes() const {
    return nodes_;
}

const GraphNode::InputPortNodeConnections &
GraphNode::getInputAudioPortConnections() const {
    return input_audio_port_connections_;
}

const GraphNode::InputPortNodeConnections &GraphNode::getInputParameterChangePortConnections() const {
    return input_param_change_port_connections_;
}

const GraphNode::OutputPortNodeConnections &
GraphNode::getOutputAudioPortConnections() const {
    return output_audio_port_connections_;
}

const GraphNode::OutputPortNodeConnections &GraphNode::getOutputParameterChangePortConnections() const {
    return output_param_change_port_connections_;
}

int GraphNode::getAudioInputPortSize() const {
    return input_audio_port_connections_.size();
}
int GraphNode::getAudioOutputPortSize() const {
    return output_audio_port_connections_.size();
}

int GraphNode::getParameterChangeInputPortSize() const {
    return input_param_change_port_connections_.size();
}

int GraphNode::getParameterChangeOutputPortSize() const {
    return output_param_change_port_connections_.size();
}

int GraphNode::getAudioInputPortChannels(int port_index) const {
    const auto &first_port_connection =
        input_audio_port_connections_.at(port_index).front();
    const int node_port_index = first_port_connection.node_port_index;
    return first_port_connection.node->getAudioInputPortChannels(
        node_port_index);
}
int GraphNode::getAudioOutputPortChannels(int port_index) const {
    const auto &node_in_port_index =
        output_audio_port_connections_.at(port_index).node;
    const int node_port_index =
        output_audio_port_connections_.at(port_index).node_port_index;
    return node_in_port_index->getAudioOutputPortChannels(node_port_index);
}
void GraphNode::setState(uint8_t *state, size_t size) {
    (void)(state);
    (void)(size);
}
std::vector<uint8_t> GraphNode::getState() const {
    nlohmann::json state_json;
    state_json["node_id"] = getNodeID();
    state_json["version"] = LIBAA_VERSION;
    state_json["node_type"] = "graph_node";

    state_json["ports"] = serializePortsConnectionToJson(
        input_audio_port_connections_,
        input_param_change_port_connections_,
        output_audio_port_connections_,
        output_param_change_port_connections_);

    auto state_string = nlohmann::to_string(state_json);
    return std::vector<uint8_t>{state_string.begin(), state_string.end()};
}
} // namespace libaa