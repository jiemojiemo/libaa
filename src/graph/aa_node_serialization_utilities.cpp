//
// Created by user on 6/10/22.
//

#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/core/aa_json_utilities.h"
#include "libaa/graph/aa_i_node.h"
#include <fstream>
namespace libaa {
namespace NodeSerializationUtilities {
void nodeSaveToJsonFile(const INode *node, const std::string &file_name) {
    std::ofstream out_file(file_name);
    if (out_file.is_open()) {
        auto state_str = JsonUtilities::binaryDataToJson(node->getState());
        out_file << state_str;
    }
}

} // namespace NodeSerializationUtilities
} // namespace libaa