
//
// Created by William.Hua on 2021/3/19.
//
#include "libaa/fileio/aa_file_stream.h"
#include <gmock/gmock.h>
#include <memory>
#include <iostream>
#include "aa_test_helper.h"

using namespace std;
using namespace testing;
using namespace libaa;

class AFileStream : public Test
{
public:

    void SetUp() override
    {
        scope_file = std::make_unique<ScopeTextFile>(file_path, test_txt);
        scope_file2 = std::make_unique<ScopeTextFile>(file_path_2, test_txt2);
    }

    void TearDown() override
    {
        scope_file = nullptr;
        scope_file2 = nullptr;
    }

    const string bad_file_path = "bad_file.txt";
    const std::string file_path = "file_stream_test.txt";
    const std::string file_path_2 = "file_stream_test2.txt";
    const string test_txt = "abcdefg";
    const string test_txt2 = "abcdefghijk";
    string buf;
    std::unique_ptr<ScopeFile> scope_file;
    std::unique_ptr<ScopeFile> scope_file2;

    FileStream file_stream;
};

TEST_F(AFileStream, InCloseStatusWhenInit)
{
    ASSERT_FALSE(file_stream.isOpen());
}

TEST_F(AFileStream, ReturnNot0IfOpenFailed)
{
    int ret = file_stream.open(bad_file_path);

    ASSERT_THAT(ret, Not(0));
}

TEST_F(AFileStream, Return0IfOpenSuccessfully)
{
    int ret = file_stream.open(file_path);

    ASSERT_THAT(ret, Eq(0));
}

TEST_F(AFileStream, ReturnsTrueIfIsOpened)
{
    file_stream.open(file_path);

    ASSERT_TRUE(file_stream.isOpen());
}

TEST_F(AFileStream, ReturnsFalseIfOpenFailed)
{
    file_stream.open(bad_file_path);

    ASSERT_FALSE(file_stream.isOpen());
}

TEST_F(AFileStream, CanInitWithFilePath)
{
    FileStream file_stream(file_path);
}

TEST_F(AFileStream, InitWithFilePathWillOpenFile)
{
    FileStream file_stream(file_path);

    ASSERT_TRUE(file_stream.isOpen());
}

TEST_F(AFileStream, IsNotOpenIfInitWithInvalidFilePath)
{
    FileStream file_stream(bad_file_path);

    ASSERT_FALSE(file_stream.isOpen());
}

TEST_F(AFileStream, NotOpenAfterClose)
{
    file_stream.open(file_path);
    ASSERT_TRUE(file_stream.isOpen());

    file_stream.close();
    ASSERT_FALSE(file_stream.isOpen());
}

TEST_F(AFileStream, DestructorWillCloseFile)
{
    auto* file_stream = new FileStream();
    delete file_stream;
}

TEST_F(AFileStream, CanReadSomething)
{
    buf.resize(test_txt.size());
    file_stream.open(file_path);

    file_stream.read((uint8_t*)buf.data(), test_txt.size());

    ASSERT_THAT(test_txt, Eq(buf));
}

TEST_F(AFileStream, ReadReturnsNeg1IfInCloseStatus)
{
    buf.resize(test_txt.size());
    ASSERT_FALSE(file_stream.isOpen());

    int ret = file_stream.read((uint8_t*)buf.data(), test_txt.size());

    ASSERT_THAT(ret, Eq(-1));
}

TEST_F(AFileStream, ReadReturnNumberOfBytesReallyRead)
{
    int a_big_size = 1000;
    buf.resize(a_big_size);

    file_stream.open(file_path);
    int ret = file_stream.read((uint8_t*)buf.data(), a_big_size);

    ASSERT_THAT(ret, Eq(test_txt.size()));
}

TEST_F(AFileStream, InitWithLength0)
{
    ASSERT_THAT(file_stream.length(), Eq(0));
}

TEST_F(AFileStream, CanGetLengthAfterOpenSuccessfully)
{
    file_stream.open(file_path);
    ASSERT_TRUE(file_stream.isOpen());

    ASSERT_THAT(file_stream.length(), Eq(test_txt.size()));
}

TEST_F(AFileStream, CloseWillSetLengthTo0)
{
    file_stream.open(file_path);
    file_stream.close();

    ASSERT_THAT(file_stream.length(), Eq(0));
}

TEST_F(AFileStream, ContinueOpenWillCloseLastFile)
{
    file_stream.open(file_path);
    file_stream.open(file_path_2);

    ASSERT_TRUE(file_stream.isOpen());
    ASSERT_THAT(file_stream.length(), Eq(test_txt2.size()));
}

TEST_F(AFileStream, TellgReturnPosition0WhenInit)
{
    ASSERT_THAT(file_stream.tellg(), Eq(0));
}

TEST_F(AFileStream, ReadWillIncreasePosition)
{
    const int num_bytes_to_read = 3;
    buf.resize(num_bytes_to_read);

    file_stream.open(file_path);
    file_stream.read((uint8_t*)buf.data(), num_bytes_to_read);

    ASSERT_THAT(file_stream.tellg(), Eq(num_bytes_to_read));
}

TEST_F(AFileStream, TellgReturnPositon0InCloseStatus)
{
    const int num_bytes_to_read = 3;
    buf.resize(num_bytes_to_read);
    file_stream.open(file_path);
    file_stream.read((uint8_t*)buf.data(), num_bytes_to_read);
    ASSERT_THAT(file_stream.tellg(), Not(0));

    file_stream.close();

    ASSERT_THAT(file_stream.tellg(), Eq(0));
}

TEST_F(AFileStream, PositionChangedAfterSeekg)
{
    int new_pos = 3;
    file_stream.open(file_path);
    file_stream.seekg(new_pos, SEEK_SET);

    ASSERT_THAT(file_stream.tellg(), Eq(new_pos));
}

TEST_F(AFileStream, SeekgReturnNeg1IfInCloseStatus)
{
    int ret = file_stream.seekg(0, SEEK_SET);
    ASSERT_THAT(ret, Eq(-1));
}