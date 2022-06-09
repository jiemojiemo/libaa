//
// Created by user on 5/17/22.
//
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/processor/aa_processor_factory.h"
#include <filesystem>
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ANodeSerializationUtilities : public Test {
public:
    auto buildTestNode() {
        std::shared_ptr<IAudioProcessor> proc = ProcessorFactory::create("Gain");
        return std::make_shared<ProcessorNode>(proc);
    }
};

TEST_F(ANodeSerializationUtilities, CanSaveNodeToLocalFile) {
    auto node = buildTestNode();
    auto output_file_path = "node.json";
    NodeSerializationUtilities::nodeSaveToJsonFile(node.get(), output_file_path);

    ASSERT_TRUE(std::filesystem::exists(output_file_path));
    std::filesystem::remove(output_file_path);
}