//
// Created by user on 5/9/22.
//

#ifndef LIBAA_AA_CONNECTION_H
#define LIBAA_AA_CONNECTION_H

#pragma once
#include "libaa/graph/aa_i_node.h"
namespace libaa {
class Connection {
public:
    Connection(std::shared_ptr<INode> upstream_n,
               int upstream_port_inx,
               int downstream_port_inx)
        : upstream_node(std::move(upstream_n)),
          upstream_port_index(upstream_port_inx),
          downstream_port_index(downstream_port_inx) {
    }

    std::shared_ptr<INode> upstream_node{nullptr};
    int upstream_port_index{-1};
    int downstream_port_index{-1};
};
} // namespace libaa
#endif // LIBAA_AA_CONNECTION_H
