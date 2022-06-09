//
// Created by user on 5/13/22.
//

#ifndef LIBAA_AA_GRAPH_BUILDER_H
#define LIBAA_AA_GRAPH_BUILDER_H

#pragma once
#include "libaa/core/aa_json_utilities.h"
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_graph_node.h"
#include "libaa/graph/aa_i_node.h"
#include "libaa/graph/aa_node_utilities.h"
#include "libaa/graph/aa_port.h"

namespace libaa {

class GraphBuilder {
public:
    bool insert(const std::string &node_id, std::shared_ptr<INode> node);

    bool addConnection(ConnectionType type,
                       std::pair<std::shared_ptr<INode>, int> upstream_node_port,
                       std::pair<std::shared_ptr<INode>, int> downstream_node_port);
    bool addConnection(ConnectionType type,
                       const std::pair<std::string, int> &upstream_node_port,
                       const std::pair<std::string, int> &downstream_node_port);

    void exposePort(PortDirection direction, PortType type, int graph_port, const std::pair<std::shared_ptr<INode>, int> &node_and_port);
    void exposePort(PortDirection direction, PortType type, int graph_port, const std::pair<std::string, int> &node_and_port);

    std::unique_ptr<INode> build() const;

    const std::vector<std::shared_ptr<INode>> &getNodes() const;

    const std::map<int, std::list<GraphNode::PortNodeConnection>> &getExposedInputAudioPorts() const;

    const std::map<int, std::list<GraphNode::PortNodeConnection>> &getExposedInputParameterChangePorts() const;

    const std::map<int, GraphNode::PortNodeConnection> &getExposedOutputAudioPorts() const;

    const std::map<int, GraphNode::PortNodeConnection> &getExposedOutputParameterChangePorts() const;

private:
    std::vector<std::shared_ptr<INode>> nodes_;
    std::map<int, std::list<GraphNode::PortNodeConnection>> input_audio_ports_map_;
    std::map<int, std::list<GraphNode::PortNodeConnection>> input_pc_ports_map_;
    std::map<int, GraphNode::PortNodeConnection> output_audio_ports_map_;
    std::map<int, GraphNode::PortNodeConnection> output_pc_ports_map_;
};
} // namespace libaa

#endif // LIBAA_AA_GRAPH_BUILDER_H
