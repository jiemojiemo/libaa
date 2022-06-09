//
// Created by user on 6/10/22.
//

#ifndef LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H
#define LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H

#include <nlohmann/json.hpp>
namespace libaa {
class INode;
namespace NodeSerializationUtilities {
void nodeSaveToJsonFile(const INode *node, const std::string &file_name);
} // namespace NodeSerializationUtilities
} // namespace libaa

#endif // LIBAA_AA_NODE_SERIALIZATION_UTILITIES_H
