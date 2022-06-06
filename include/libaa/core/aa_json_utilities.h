//
// Created by user on 5/12/22.
//

#ifndef LIBAA_AA_JSON_UTILITIES_H
#define LIBAA_AA_JSON_UTILITIES_H

#pragma once
#include "nlohmann/json.hpp"
namespace libaa {
namespace JsonUtilities {
std::vector<uint8_t> jsonToBinaryData(const nlohmann::json &j);
nlohmann::json binaryDataToJson(const std::vector<uint8_t> &data);
nlohmann::json binaryDataToJson(uint8_t *data, size_t size);
} // namespace JsonUtilities
} // namespace libaa

#endif // LIBAA_AA_JSON_UTILITIES_H
