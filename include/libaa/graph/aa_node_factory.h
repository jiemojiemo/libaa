//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AA_NODE_FACTORY_H
#define LIBAA_AA_NODE_FACTORY_H

#pragma once
#include <memory>
namespace libaa {
class INode;
class ResourcePathFinder;
namespace NodeFactory {
std::unique_ptr<INode> build(uint8_t *state,
                             size_t size,
                             const ResourcePathFinder *finder = nullptr);

std::unique_ptr<INode> buildFromFilePath(const std::string &file_path,
                                         const ResourcePathFinder *finder = nullptr);
} // namespace NodeFactory
} // namespace libaa

#endif // LIBAA_AA_NODE_FACTORY_H
