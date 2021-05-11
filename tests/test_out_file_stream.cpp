
//
// Created by William.Hua on 2021/3/21.
//
#include <string>
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "aa_test_helper.h"
#include <vector>
#include <gmock/gmock.h>
using namespace testing;
using namespace std;
using namespace libaa;



class AOutFileStream : public Test
{
public:
    void SetUp() override
    {
        scope_file = std::make_unique<ScopeTextFile>(test_out_path, "");
    }

    void TearDown() override
    {
        scope_file = nullptr;
    }
    const string test_out_path = "ostream_test.txt";
    const string test_txt = "abcdefg";

    const string bad_path = "/abc/bad_path.txt";

    std::unique_ptr<ScopeFile> scope_file;
    FileOutputStream out_stream;
};


TEST_F(AOutFileStream, InCloseStatusWhenInit)
{
    ASSERT_FALSE(out_stream.isOpen());
}


TEST_F(AOutFileStream, ReturnNot0IfOpenFailed)
{
    int ret = out_stream.open(bad_path);

    ASSERT_THAT(ret, Not(0));
}

TEST_F(AOutFileStream, Return0IfOpenSuccessfully)
{
    int ret = out_stream.open(test_out_path);

    ASSERT_THAT(ret, Eq(0));
}

TEST_F(AOutFileStream, InitWithOutputPathWillOpenFile)
{
    FileOutputStream out_stream(test_out_path);

    ASSERT_TRUE(out_stream.isOpen());
}

TEST_F(AOutFileStream, InitWithBadOutputPathWillOpenFileFailed)
{
    FileOutputStream out_stream(bad_path);

    ASSERT_FALSE(out_stream.isOpen());
}

TEST_F(AOutFileStream, NoLongerOpenAfterClose)
{
    out_stream.open(test_out_path);
    ASSERT_TRUE(out_stream.isOpen());

    out_stream.close();
    ASSERT_FALSE(out_stream.isOpen());
}

TEST_F(AOutFileStream, DestructorWillCloseFile)
{
    auto* out_stream = new FileOutputStream;
    delete out_stream;
}

TEST_F(AOutFileStream, CanWriteSomething)
{
    out_stream.open(test_out_path);

    out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());
    out_stream.close();

    auto in_stream = std::make_unique<FileInputStream>(test_out_path);
    string dst_buffer(test_txt.size(),'\0');
    in_stream->read((uint8_t*)(dst_buffer.data()), dst_buffer.size());

    ASSERT_THAT(dst_buffer, Eq(test_txt));
}

TEST_F(AOutFileStream, WriteReturnNeg1IfInCloseStatus)
{
    int ret = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());

    ASSERT_THAT(ret, Eq(-1));
}

TEST_F(AOutFileStream, WriteReturnNumberOfBytesWritten)
{
    out_stream.open(test_out_path);
    int written = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());

    ASSERT_THAT(written, Eq(test_txt.size()));
}

TEST_F(AOutFileStream, WriteIncreasePosition)
{
    out_stream.open(test_out_path);
    int written = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());

    ASSERT_THAT(out_stream.tellp(), Eq(written));
}

TEST_F(AOutFileStream, TellpReturnPosition0IfInCloseStatus)
{
    out_stream.open(test_out_path);
    int written = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());
    ASSERT_THAT(out_stream.tellp(), Eq(written));

    out_stream.close();
    ASSERT_THAT(out_stream.tellp(), Eq(0));
}

TEST_F(AOutFileStream, PositionChangedAfterSeekp)
{
    int new_pos = 3;
    out_stream.open(test_out_path);
    out_stream.seekp(new_pos, SEEK_SET);

    ASSERT_THAT(out_stream.tellp(), Eq(new_pos));
}

TEST_F(AOutFileStream, SeekpReturnNeg1IfInCloseStatus)
{
    int ret = out_stream.seekp(0, SEEK_SET);
    ASSERT_THAT(ret, Eq(-1));
}

TEST_F(AOutFileStream, InitWithLength0)
{
    ASSERT_THAT(out_stream.length(), Eq(0));
}

TEST_F(AOutFileStream, WriteToEndWillIncreaseLength)
{
    out_stream.open(test_out_path);
    int written0 = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());
    int written1 = out_stream.write((uint8_t*)(test_txt.data()), test_txt.size() - 1);

    ASSERT_THAT(out_stream.length(), Eq(written0 + written1));
}

TEST_F(AOutFileStream, LenghtIsZeroIfInCloseStatus)
{
    out_stream.open(test_out_path);
    out_stream.write((uint8_t*)(test_txt.data()), test_txt.size());
    ASSERT_THAT(out_stream.length(), Not(0));

    out_stream.close();

    ASSERT_THAT(out_stream.length(), Eq(0));
}

