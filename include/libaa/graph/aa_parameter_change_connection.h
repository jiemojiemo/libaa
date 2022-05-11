//
// Created by user on 5/9/22.
//

#ifndef LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H
#define LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H

#pragma once
#include "libaa/graph/aa_connection.h"
#include "libaa/graph/aa_i_node.h"
#include "libaa/graph/aa_parameter_change_port.h"

namespace libaa {
class ParameterChangeConnection : public Connection {
public:
    ParameterChangeConnection(std::shared_ptr<INode> upstream_n,
                              int upstream_port_inx,
                              int downstream_port_inx)
        : Connection(std::move(upstream_n), upstream_port_inx, downstream_port_inx, ConnectionType::kParameterChangeConnection) {
    }

    ParameterChangePort &pull() {
        return upstream_node->pullParameterChangePort(upstream_port_index);
    }
};

bool operator==(const ParameterChangeConnection &lhs, const ParameterChangeConnection &rhs);
} // namespace libaa

#endif // LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H
