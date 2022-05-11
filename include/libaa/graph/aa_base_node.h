//
// Created by user on 5/11/22.
//

#ifndef LIBAA_AA_BASE_NODE_H
#define LIBAA_AA_BASE_NODE_H

#pragma once
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_i_node.h"
#include "libaa/graph/aa_parameter_change_connection.h"
namespace libaa {
class BaseNode : public INode {
public:
    virtual ~BaseNode() override = default;

    void setNodeID(std::string node_id) override;

    std::string getNodeID() const override;

    const std::vector<AudioConnection> &getUpstreamAudioConnections() const;

    const std::vector<ParameterChangeConnection> &getUpstreamParameterConnections() const;

protected:
    std::string node_id_{};
    std::vector<AudioConnection> audio_connections_;
    std::vector<ParameterChangeConnection> param_change_connections_;
};
} // namespace libaa

#endif // LIBAA_AA_BASE_NODE_H
