//
// Created by William on 3/28/22.
//

#ifndef AUDIOEFFECT_AE_DEMO_CONTEXT_H
#define AUDIOEFFECT_AE_DEMO_CONTEXT_H

#pragma once
#include "libaa/core/aa_resource_path_finder.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <yaml-cpp/yaml.h>
class DemoContext {
public:
    static std::vector<DemoContext> loadAll(const std::string &input) {
        try {
            std::vector<YAML::Node> nodes = YAML::LoadAll(input);
            std::vector<DemoContext> contexts(nodes.size());
            for (auto i = 0u; i < nodes.size(); ++i) {
                contexts[i] = nodes[i].as<DemoContext>();
            }
            return contexts;
        } catch (const std::exception &e) {
            std::cerr << "parse yaml failed: " << e.what() << std::endl;
            return {};
        }
    }

    static std::vector<DemoContext>
    loadAllFromFile(const std::string &filename) {
        std::ifstream fin(filename);
        if (!fin) {
            throw std::invalid_argument(filename);
        }
        std::string content((std::istreambuf_iterator<char>(fin)),
                            (std::istreambuf_iterator<char>()));
        return loadAll(content);
    }

    std::string demo_name;

    libaa::ResourcePathFinder finder;
    std::vector<std::string> search_paths;

    std::string input_file_name;
    int in_sample_rate = 0;
    int in_num_channels = 0;
    int in_num_frames = 0;
    std::vector<std::vector<float>> in_samples;

    int block_size = 1024;
    int current_round = 0;
    int num_round = 1;

    std::string output_dir = "./";
    std::string output_file_name;
    std::string output_full_path;
};

namespace YAML {
template <> struct convert<DemoContext> {
    template <typename T>
    static T getValueFromNode(const Node &node, const std::string &key,
                              T default_value) {
        if (node[key].IsDefined()) {
            return node[key].as<T>();
        }
        return default_value;
    }
    static void decodeSearchPaths(const Node &node, DemoContext &rhs) {
        if (node["search_paths"].IsDefined() &&
            node["search_paths"].IsSequence()) {
            for (size_t i = 0; i < node["search_paths"].size(); ++i) {
                rhs.search_paths.emplace_back(
                    node["search_paths"][i].as<std::string>());

                rhs.finder.append(rhs.search_paths.back());
            }
        }
    }
    static bool decode(const Node &node, DemoContext &rhs) {
        if (!node.IsDefined() || !node.IsMap()) {
            return false;
        }

        rhs.demo_name = getValueFromNode(node, "demo_name", rhs.demo_name);
        rhs.input_file_name =
            getValueFromNode(node, "input_file_name", rhs.input_file_name);

        decodeSearchPaths(node, rhs);

        rhs.output_dir = getValueFromNode(node, "output_dir", rhs.output_dir);
        rhs.output_file_name =
            getValueFromNode(node, "output_file_name", rhs.output_file_name);

        rhs.block_size = getValueFromNode(node, "block_size", rhs.block_size);

        return true;
    }
};
} // namespace YAML

#endif // AUDIOEFFECT_AE_DEMO_CONTEXT_H
