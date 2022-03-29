//
// Created by user on 3/29/22.
//

#ifndef LIBAA_AA_RESOURCE_PATH_FINDER_H
#define LIBAA_AA_RESOURCE_PATH_FINDER_H

#pragma once
#include <string>
#include <vector>
namespace libaa {
class ResourcePathFinder {
public:
    ResourcePathFinder();

    void append(std::string p);

    const std::vector<std::string> &getSearchPaths() const;

    std::string find(const std::string &file_name) const;

private:
    std::vector<std::string> paths_;
};
} // namespace libaa

#endif // LIBAA_AA_RESOURCE_PATH_FINDER_H
