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

nlohmann::json binaryDataToJson(const std::vector<uint8_t> &data) {
    if (!data.empty()) {
        return nlohmann::json::parse(data.begin(), data.end());
    }

    return nlohmann::json{};
}

nlohmann::json binaryDataToJson(uint8_t *data, size_t size) {
    if (data && size > 0) {
        return nlohmann::json::parse(data, data + size);
    }

    return nlohmann::json{};
}

} // namespace NodeSerializationUtilities
} // namespace libaa