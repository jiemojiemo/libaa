//
// Created by William on 3/28/22.
//
#include "../aa_testing_helper.h"
#include "ae_demo_context.h"
#include <gmock/gmock.h>
using namespace testing;

class ADemoContext : public Test {
public:
    const std::string test_string =
        R"(
---
demo_name: dog
search_paths:
 - /path/a
 - /path/b
input_file_name: abc.wav
output_dir: /xxx/
output_file_name: xyz.wav
block_size: 123
---
demo_name: cat
)";
};

TEST_F(ADemoContext, CanLoadFromYAMLString) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);
}

TEST_F(ADemoContext, ReturnsEmptyContextsIfStringInvalid) {
    std::string invalid_str = "xxx";
    std::vector<DemoContext> contexts = DemoContext::loadAll(invalid_str);

    ASSERT_THAT(contexts.size(), Eq(0));
}

TEST_F(ADemoContext, CanLoadMultiCases) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts.size(), Eq(2));
}

TEST_F(ADemoContext, CanLoadFromYAMLFilePath) {
    std::string file_path = "./test.yaml";
    libaa::ScopeTextFile scope_file(file_path, test_string);

    std::vector<DemoContext> contexts = DemoContext::loadAllFromFile(file_path);

    ASSERT_THAT(contexts.size(), Eq(2));
    ASSERT_THAT(contexts[0].demo_name, Eq("dog"));
}

TEST_F(ADemoContext, CanGetDemoName) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].demo_name, Eq("dog"));
}
TEST_F(ADemoContext, CanGetSearchPaths) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].search_paths.size(), Eq(2));
}

TEST_F(ADemoContext, CanGetInputFile) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].input_file_name, Eq("abc.wav"));
}

TEST_F(ADemoContext, CanGetOutputFileName) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].output_file_name, Eq("xyz.wav"));
}

TEST_F(ADemoContext, CanGetOutputFileDir) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].output_dir, Eq("/xxx/"));
}

TEST_F(ADemoContext, CanGetBlockSize) {
    std::vector<DemoContext> contexts = DemoContext::loadAll(test_string);

    ASSERT_THAT(contexts[0].block_size, Eq(123));
}
