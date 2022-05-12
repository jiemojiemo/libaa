//
// Created by user on 5/12/22.
//

#include "libaa/graph/aa_node_serialization_utilities.h"

namespace libaa {
namespace NodeSerializationUtilities {
std::vector<uint8_t> jsonToBinaryData(const nlohmann::json &j) {
    auto j_str = nlohmann::to_string(j);
    return std::vector<uint8_t>(j_str.begin(), j_str.end());
}
} // namespace NodeSerializationUtilities
} // namespace libaa