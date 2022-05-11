//
// Created by user on 5/11/22.
//

#include "libaa/graph/aa_base_node.h"

namespace libaa {

void BaseNode::setNodeID(std::string node_id) {
    node_id_ = std::move(node_id);
}

std::string BaseNode::getNodeID() const {
    return node_id_;
}

const std::vector<AudioConnection> &BaseNode::getUpstreamAudioConnections() const {
    return audio_connections_;
}

const std::vector<ParameterChangeConnection> &BaseNode::getUpstreamParameterConnections() const {
    return param_change_connections_;
}

} // namespace libaa