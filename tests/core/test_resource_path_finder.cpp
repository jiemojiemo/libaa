//
// Created by user on 3/29/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_resource_path_finder.h"
#include <filesystem>
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class AResourceFinder : public Test {
public:
    ResourcePathFinder finder;
};

TEST_F(AResourceFinder, HasCurrentDirSearchPathsWhenInit) {
    ASSERT_THAT(finder.getSearchPaths().size(), Eq(1));
    ASSERT_THAT(finder.getSearchPaths()[0], Eq("./"));
}

TEST_F(AResourceFinder, SearchPathSizeIncreasedAfterAppend) {
    finder.append("/abc/xyz");

    ASSERT_THAT(finder.getSearchPaths().size(), Eq(2));
}

TEST_F(AResourceFinder, ReturnsFullPathIfFindFile) {
    std::string file_name = "resource_finder_text.txt";
    std::string save_dir = "./";
    std::string save_path = save_dir + file_name;
    ScopeTextFile scope_file(save_path, "this is content");

    finder.append(save_dir);

    std::filesystem::path file_path = finder.find(file_name);

    ASSERT_THAT(file_path.string(), Eq(save_path));
}

TEST_F(AResourceFinder, ReturnEmptyPathIfCannotFindPath) {
    std::string file_name = "not_such_file.txt";
    std::string save_dir = "./";

    finder.append(save_dir);

    std::filesystem::path file_path = finder.find(file_name);

    ASSERT_TRUE(file_path.empty());
}