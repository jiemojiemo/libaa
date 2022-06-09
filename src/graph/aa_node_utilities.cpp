//
// Created by user on 5/13/22.
//
#include "libaa/graph/aa_node_utilities.h"
#include "libaa/graph/aa_i_node.h"

namespace libaa {
namespace NodeUtilities {
std::shared_ptr<INode> findNodeById(const std::vector<std::shared_ptr<INode>> &nodes, const std::string &node_id) {
    for (auto &node : nodes) {
        if (node->getNodeID() == node_id)
            return node;
    }
    return nullptr;
}

ConnectionType getConnectionTypeFromString(const std::string &connection_type) {
    if (connection_type == "audio") {
        return ConnectionType::kAudioConnection;
    } else if (connection_type == "parameter_change") {
        return ConnectionType::kParameterChangeConnection;
    } else {
        throw std::invalid_argument(connection_type + " type invalid");
    }
}

PortDirection getPortDirectionTypeFromString(const std::string &direction_type) {
    if (direction_type == "input") {
        return PortDirection::kInput;
    } else if (direction_type == "output") {
        return PortDirection::kOutput;
    } else {
        throw std::invalid_argument(direction_type + " type invalid");
    }
}

PortType getPortTypeFromString(const std::string &port_type) {
    if (port_type == "audio") {
        return PortType::kAudio;
    } else if (port_type == "parameter_change") {
        return PortType::kParameterChange;
    } else {
        throw std::invalid_argument(port_type + " type invalid");
    }
}

std::string connectionTypeToString(ConnectionType connection_type) {
    switch (connection_type) {
    case ConnectionType::kAudioConnection:
        return "audio";
    case ConnectionType::kParameterChangeConnection:
        return "parameter_change";
    }
}

std::string portDirectionTypeToString(PortDirection direction_type) {
    switch (direction_type) {
    case PortDirection::kInput:
        return "input";
    case PortDirection::kOutput:
        return "output";
    }
}

std::string portTypeToString(PortType port_type) {
    switch (port_type) {
    case PortType::kAudio:
        return "audio";
    case PortType::kParameterChange:
        return "parameter_change";
    }
}

} // namespace NodeUtilities
} // namespace libaa