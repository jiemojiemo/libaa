//
// Created by user on 3/29/22.
//

#include "libaa/core/aa_resource_path_finder.h"
#include <filesystem>
using namespace std;
namespace libaa {
ResourcePathFinder::ResourcePathFinder() {
    paths_.emplace_back("./");
}
void ResourcePathFinder::append(std::string p) {
    paths_.push_back(p);
}

const std::vector<std::string> &ResourcePathFinder::getSearchPaths() const {
    return paths_;
}

std::string ResourcePathFinder::find(const std::string &file_name) const {
    // try to find file name in search paths
    for (const auto &p : paths_) {
        auto try_find_path = filesystem::path(p) / file_name;
        if (filesystem::exists(try_find_path)) {
            return try_find_path.string();
        }
    }
    return {};
}

} // namespace libaa