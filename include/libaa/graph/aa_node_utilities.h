//
// Created by user on 5/13/22.
//

#ifndef LIBAA_AA_NODE_UTILITES_H
#define LIBAA_AA_NODE_UTILITES_H

#pragma once
#include "libaa/graph/aa_connection.h"
#include "libaa/graph/aa_port.h"
#include <memory>
#include <vector>
namespace libaa {
class INode;
namespace NodeUtilities {
std::shared_ptr<INode> findNodeById(const std::vector<std::shared_ptr<INode>> &nodes, const std::string &node_id);

ConnectionType getConnectionTypeFromString(const std::string &connection_type);

PortDirection getPortDirectionTypeFromString(const std::string &direction_type);

PortType getPortTypeFromString(const std::string &port_type);

std::string connectionTypeToString(ConnectionType connection_type);

std::string portDirectionTypeToString(PortDirection direction_type);

std::string portTypeToString(PortType port_type);

} // namespace NodeUtilities
} // namespace libaa

#endif // LIBAA_AA_NODE_UTILITES_H
