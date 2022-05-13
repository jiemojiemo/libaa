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

} // namespace NodeUtilities
} // namespace libaa
