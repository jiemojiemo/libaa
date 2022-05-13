//
// Created by user on 5/13/22.
//

#ifndef LIBAA_AA_NODE_UTILITES_H
#define LIBAA_AA_NODE_UTILITES_H

#pragma once
#include <memory>
#include <vector>
namespace libaa {
class INode;
namespace NodeUtilities {
std::shared_ptr<INode> findNodeById(const std::vector<std::shared_ptr<INode>> &nodes, const std::string &node_id);
}
} // namespace libaa

#endif // LIBAA_AA_NODE_UTILITES_H
