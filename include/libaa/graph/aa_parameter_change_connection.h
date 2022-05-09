//
// Created by user on 5/9/22.
//

#ifndef LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H
#define LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H

#pragma once
#include "libaa/graph/aa_i_node.h"
#include "libaa/graph/aa_parameter_change_port.h"

namespace libaa {
class ParameterChangeConnection {
public:
    ParameterChangeConnection(std::shared_ptr<INode> upstream_n,
                              int upstream_audio_port_inx,
                              int downstream_audio_port_inx)
        : upstream_node(std::move(upstream_n)),
          upstream_param_change_port_index(upstream_audio_port_inx),
          downstream_param_change_port_index(downstream_audio_port_inx) {
    }

    ParameterChangePort &pull() {
        return upstream_node->pullParameterChangePort(upstream_param_change_port_index);
    }

    std::shared_ptr<INode> upstream_node{nullptr};
    int upstream_param_change_port_index{-1};
    int downstream_param_change_port_index{-1};
};
} // namespace libaa

#endif // LIBAA_AA_PARAMETER_CHANGE_CONNECTION_H
