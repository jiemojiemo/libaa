//
// Created by user on 5/7/22.
//
#include "libaa/graph/aa_graph_node.h"
#include "libaa/aa_version.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/graph/aa_node_utilities.h"
#include "libaa/graph/aa_parameter_change_connection.h"
#include "libaa/graph/aa_port.h"
#include "libaa/processor/aa_processor_factory.h"
#include <nlohmann/json.hpp>

namespace libaa {

auto portTypeToString(PortType port_type) {
    if (port_type == PortType::kAudio) {
        return "audio";
    } else {
        return "parameter_change";
    }
}

auto serializePortConnectionToJson(const GraphNode::PortNodeConnection &port_con,
                                   PortDirection direction,
                                   PortType port_type,
                                   int graph_port_index) {
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
    port_json["port_type"] = portTypeToString(port_type);
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

auto serializeNodeConnectionToJson(const Connection &con, const std::shared_ptr<INode> &downstream_node, PortType type) {
    nlohmann::json connection_json;
    connection_json["upstream_node_id"] = con.upstream_node->getNodeID();
    connection_json["upstream_node_port"] = con.upstream_port_index;
    connection_json["downstream_node_port"] = con.downstream_port_index;
    connection_json["downstream_node_id"] = downstream_node->getNodeID();
    connection_json["port_type"] = portTypeToString(type);
    return connection_json;
}

auto serializeNodeConnectionsToJson(const std::vector<std::shared_ptr<INode>> &nodes) {

    nlohmann::json connections_json;
    // audio connections
    for (const auto &node : nodes) {
        const auto &upstream_audio_connections = node->getUpstreamAudioConnections();
        const auto &upstream_pc_connections = node->getUpstreamParameterConnections();

        for (const auto &connection : upstream_audio_connections) {
            connections_json.push_back(serializeNodeConnectionToJson(connection, node, PortType::kAudio));
        }

        for (const auto &connection : upstream_pc_connections) {
            connections_json.push_back(serializeNodeConnectionToJson(connection, node, PortType::kParameterChange));
        }
    }

    return connections_json;
}

auto serializeNodesToJson(const std::vector<std::shared_ptr<INode>> &nodes) {
    nlohmann::json nodes_json;
    for (const auto &n : nodes) {
        auto node_json = NodeSerializationUtilities::binaryDataToJson(n->getState());
        nodes_json.push_back(node_json);
    }
    return nodes_json;
}

auto buildNodesFromJson(const nlohmann::json &nodes_json) {
    std::vector<std::shared_ptr<INode>> nodes;

    for (auto &node_json : nodes_json) {
        auto node_type = node_json["node_type"].get<std::string>();
        auto node_state = NodeSerializationUtilities::jsonToBinaryData(node_json);
        if (node_type == "processor_node") {
            auto proc_node = std::make_shared<ProcessorNode>(nullptr);
            proc_node->setState(node_state.data(), node_state.size());

            nodes.push_back(proc_node);
        } else if (node_type == "graph_node") {
            auto graph_node = std::shared_ptr<GraphNode>(new GraphNode({}, {}, {}));
            graph_node->setState(node_state.data(), node_state.size());

            nodes.push_back(graph_node);
        } else {
            throw std::invalid_argument(node_type + " is unsupported node type");
        }
    }

    return nodes;
}

auto buildPortsFromJson(const nlohmann::json &ports_json,
                        GraphNode::InputPortNodeConnections &input_audio_port_connections,
                        GraphNode::InputPortNodeConnections &input_param_change_port_connections,
                        GraphNode::OutputPortNodeConnections &output_audio_port_connections,
                        GraphNode::OutputPortNodeConnections &output_param_change_port_connections,
                        const std::vector<std::shared_ptr<INode>> &nodes) {

    // find the max graph port index
    int max_graph_audio_input_port_index = -1;
    int max_graph_pc_input_port_index = -1;
    int max_graph_audio_output_port_index = -1;
    int max_graph_pc_output_port_index = -1;
    for (const auto &port : ports_json) {
        auto port_direction = port["port_direction"].get<std::string>();
        auto port_index = port["port_index"].get<int>();
        auto port_type = port["port_type"].get<std::string>();

        // input
        if (port_direction == "input") {
            if (port_type == "audio") {
                max_graph_audio_input_port_index = std::max(max_graph_audio_input_port_index, port_index);
            } else if (port_type == "parameter_change") {
                max_graph_pc_input_port_index = std::max(max_graph_pc_input_port_index, port_index);
            }
        }

        // output
        if (port_direction == "output") {
            if (port_type == "audio") {
                max_graph_audio_output_port_index = std::max(max_graph_audio_output_port_index, port_index);
            } else if (port_type == "parameter_change") {
                max_graph_pc_output_port_index = std::max(max_graph_pc_output_port_index, port_index);
            }
        }
    }

    if (max_graph_audio_input_port_index != -1) {
        input_audio_port_connections.resize(max_graph_audio_input_port_index + 1);
    }

    if (max_graph_pc_input_port_index != -1) {
        input_param_change_port_connections.resize(max_graph_pc_input_port_index + 1);
    }

    if (max_graph_audio_output_port_index != -1) {
        output_audio_port_connections.resize(max_graph_audio_output_port_index + 1);
    }

    if (max_graph_pc_output_port_index != -1) {
        output_param_change_port_connections.resize(max_graph_pc_output_port_index + 1);
    }

    for (const auto &port : ports_json) {
        auto internal_node_id = port["internal_node_id"].get<std::string>();
        auto internal_node_port_index = port["internal_node_port_index"].get<int>();
        auto port_direction = port["port_direction"].get<std::string>();
        auto port_index = port["port_index"].get<int>();
        auto port_type = port["port_type"].get<std::string>();

        auto internal_node = NodeUtilities::findNodeById(nodes, internal_node_id);
        GraphNode::PortNodeConnection port_node_conn{internal_node, internal_node_port_index};

        if (port_direction == "input") {
            if (port_type == "audio") {
                input_audio_port_connections[port_index].push_back(port_node_conn);
            } else if (port_type == "parameter_change") {
                input_param_change_port_connections[port_index].push_back(port_node_conn);
            }
        }

        if (port_direction == "output") {
            if (port_type == "audio") {
                output_audio_port_connections[port_index] = port_node_conn;
            } else if (port_type == "parameter_change") {
                output_param_change_port_connections[port_index] = port_node_conn;
            }
        }
    }
}

auto buildConnectionsForNodes(const nlohmann::json &connections_json, std::vector<std::shared_ptr<INode>> &nodes) {
    for (const auto &connection_json : connections_json) {
        auto downstream_node_id = connection_json["downstream_node_id"].get<std::string>();
        auto downstream_node_port = connection_json["downstream_node_port"].get<int>();
        auto upstream_node_id = connection_json["upstream_node_id"].get<std::string>();
        auto upstream_node_port = connection_json["upstream_node_port"].get<int>();
        auto port_type = connection_json["port_type"].get<std::string>();

        auto downstream_node = NodeUtilities::findNodeById(nodes, downstream_node_id);
        auto upstream_node = NodeUtilities::findNodeById(nodes, upstream_node_id);

        if (port_type == "audio") {
            AudioConnection con{upstream_node, upstream_node_port, downstream_node_port};
            downstream_node->addUpstreamAudioConnection(con);
        } else if (port_type == "parameter_change") {
            ParameterChangeConnection con{upstream_node, upstream_node_port, downstream_node_port};
            downstream_node->addUpstreamParameterChangeConnection(con);
        }
    }
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

NodeType GraphNode::getNodeType() const {
    return NodeType::kGraphNode;
}

void GraphNode::setTransportContext(std::shared_ptr<TransportContext> transport_context) {
    for (auto &n : nodes_) {
        n->setTransportContext(transport_context);
    }
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
    auto state_json = NodeSerializationUtilities::binaryDataToJson(state, size);

    BaseNode::setNodeID(state_json["node_id"].get<std::string>());

    nodes_ = buildNodesFromJson(state_json["nodes"]);

    input_audio_port_connections_.clear();
    input_param_change_port_connections_.clear();
    output_audio_port_connections_.clear();
    output_param_change_port_connections_.clear();
    buildPortsFromJson(state_json["ports"],
                       input_audio_port_connections_,
                       input_param_change_port_connections_,
                       output_audio_port_connections_,
                       output_param_change_port_connections_,
                       nodes_);

    buildConnectionsForNodes(state_json["connections"], nodes_);
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
    state_json["connections"] = serializeNodeConnectionsToJson(nodes_);
    state_json["nodes"] = serializeNodesToJson(nodes_);

    return NodeSerializationUtilities::jsonToBinaryData(state_json);
}

} // namespace libaa