//
// Created by user on 5/12/22.
//

#ifndef LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H
#define LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H

#pragma once
#include <nlohmann/json.hpp>
namespace libaa {
namespace NodeSerializationUtilities {
std::vector<uint8_t> jsonToBinaryData(const nlohmann::json &j);
}
} // namespace libaa

#endif // LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H
