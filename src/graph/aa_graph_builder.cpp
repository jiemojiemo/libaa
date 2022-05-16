//
// Created by user on 5/13/22.
//

#include "libaa/graph/aa_graph_builder.h"

namespace libaa {

namespace {

auto findMaxInputPortIndex = [](const std::map<int, std::list<GraphNode::PortNodeConnection>> &port_node_cons_map) {
    int max_port_index = -1;
    for (const auto &item : port_node_cons_map) {
        auto current_port_index = item.first;
        max_port_index = std::max(max_port_index, current_port_index);
    }
    return max_port_index;
};

auto findMaxOutputPortIndex = [](const std::map<int, GraphNode::PortNodeConnection> &port_node_cons_map) {
    int max_port_index = -1;
    for (const auto &item : port_node_cons_map) {
        auto current_port_index = item.first;
        max_port_index = std::max(max_port_index, current_port_index);
    }
    return max_port_index;
};

auto InputPortsMapToGraphNodeInputPortNodeConnections = [](const std::map<int, std::list<GraphNode::PortNodeConnection>> &port_node_cons_map) {
    auto max_graph_port_index = findMaxInputPortIndex(port_node_cons_map);

    GraphNode::InputPortNodeConnections result(max_graph_port_index + 1);
    for (const auto &item : port_node_cons_map) {
        int graph_port_index = item.first;
        result[graph_port_index] = item.second;
    }

    return result;
};

auto OutputPortsMapToOutputPortNodeConnections = [](const std::map<int, GraphNode::PortNodeConnection> &port_node_cons_map) {
    auto max_graph_port_index = findMaxOutputPortIndex(port_node_cons_map);

    GraphNode::OutputPortNodeConnections result(max_graph_port_index + 1);
    for (const auto &item : port_node_cons_map) {
        int graph_port_index = item.first;
        result[graph_port_index] = item.second;
    }

    return result;
};

auto checkPortValidAndThrows(PortDirection direction, PortType type, const std::pair<std::shared_ptr<INode>, int> &node_and_port) {
    int node_port = node_and_port.second;
    const auto &node = node_and_port.first;
    int port_size = -1;

    if (direction == PortDirection::kInput) {
        if (type == PortType::kAudio) {
            port_size = node->getAudioInputPortSize();
        } else if (type == PortType::kParameterChange) {
            port_size = node->getParameterChangeInputPortSize();
        }
    } else if (direction == PortDirection::kOutput) {
        if (type == PortType::kAudio) {
            port_size = node->getAudioOutputPortSize();
        } else if (type == PortType::kParameterChange) {
            port_size = node->getParameterChangeOutputPortSize();
        }
    }

    if (node_port >= port_size) {
        throw std::invalid_argument("invalid node port to exposed");
    }
}
} // namespace

bool GraphBuilder::insert(const std::string &node_id, std::shared_ptr<INode> node) {
    if (NodeUtilities::findNodeById(nodes_, node_id) != nullptr) {
        return false;
    }

    node->setNodeID(node_id);
    nodes_.push_back(std::move(node));
    return true;
}

bool GraphBuilder::addConnection(ConnectionType type,
                                 std::pair<std::shared_ptr<INode>, int> upstream_node_port,
                                 std::pair<std::shared_ptr<INode>, int> downstream_node_port) {
    std::shared_ptr<INode> upstream_node = upstream_node_port.first;
    int upstream_port = upstream_node_port.second;
    std::shared_ptr<INode> &downstream_node = downstream_node_port.first;
    int downstream_port = downstream_node_port.second;

    try {
        if (type == ConnectionType::kAudioConnection) {
            downstream_node->addUpstreamAudioConnection(AudioConnection{upstream_node, upstream_port, downstream_port});
        } else if (type == ConnectionType::kParameterChangeConnection) {
            downstream_node->addUpstreamParameterChangeConnection(ParameterChangeConnection{upstream_node, upstream_port, downstream_port});
        }
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

void GraphBuilder::exposePort(PortDirection direction, PortType type, int graph_port, const std::pair<std::shared_ptr<INode>, int> &node_and_port) {
    checkPortValidAndThrows(direction, type, node_and_port);

    GraphNode::PortNodeConnection port_connection{node_and_port.first, node_and_port.second};
    if (direction == PortDirection::kInput) {
        if (type == PortType::kAudio) {
            input_audio_ports_map_[graph_port].push_back(std::move(port_connection));
        } else if (type == PortType::kParameterChange) {
            input_pc_ports_map_[graph_port].push_back(std::move(port_connection));
        }
    } else if (direction == PortDirection::kOutput) {
        if (type == PortType::kAudio) {
            output_audio_ports_map_[graph_port] = port_connection;
        } else if (type == PortType::kParameterChange) {
            output_pc_ports_map_[graph_port] = port_connection;
        }
    }
}

std::shared_ptr<INode> GraphBuilder::build() const {
    return std::make_shared<GraphNode>(nodes_,
                                       InputPortsMapToGraphNodeInputPortNodeConnections(input_audio_ports_map_),
                                       InputPortsMapToGraphNodeInputPortNodeConnections(input_pc_ports_map_),
                                       OutputPortsMapToOutputPortNodeConnections(output_audio_ports_map_),
                                       OutputPortsMapToOutputPortNodeConnections(output_pc_ports_map_));
}

const std::vector<std::shared_ptr<INode>> &GraphBuilder::getNodes() const {
    return nodes_;
}

const std::map<int, std::list<GraphNode::PortNodeConnection>> &GraphBuilder::getExposedInputAudioPorts() const {
    return input_audio_ports_map_;
}

const std::map<int, std::list<GraphNode::PortNodeConnection>> &GraphBuilder::getExposedInputParameterChangePorts() const {
    return input_pc_ports_map_;
}

const std::map<int, GraphNode::PortNodeConnection> &GraphBuilder::getExposedOutputAudioPorts() const {
    return output_audio_ports_map_;
}

const std::map<int, GraphNode::PortNodeConnection> &GraphBuilder::getExposedOutputParameterChangePorts() const {
    return output_pc_ports_map_;
}
} // namespace libaa