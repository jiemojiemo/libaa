
//
// Created by William.Hua on 2021/2/9.
//
#include "libaa/fileio/aa_mp3_audio_format_reader.h"
#include "libaa/fileio/aa_file_stream.h"

#include "aa_test_helper.h"
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
using namespace testing;
using namespace libaa;
using namespace std;


class AMp3AudioFormatReader : public Test
{
public:
    void SetUp() override
    {
        left_buffer.resize(n_read_samples);
        right_buffer.resize(n_read_samples);
        third_buffer.resize(n_read_samples);
        dest[0] = left_buffer.data();
        dest[1] = right_buffer.data();
        dest[2] = third_buffer.data();
    }

    void TearDown() override
    {
    }

    const size_t sample_rate = 44100;
    const size_t num_channels = 2;
    const size_t num_frames = sample_rate * 1;
    const size_t num_bits = 16;
    const float fill_val = 0.1f;

    const int n_read_samples = 64;
    vector<float> left_buffer;
    vector<float> right_buffer;
    vector<float> third_buffer;
    float* dest[3];

    const std::string test_file_name="../../res/sound/file_example_MP3_700KB.mp3";
};

class FakeStream : public InputStream
{
public:
    ~FakeStream() override = default;
    int64_t read(uint8_t *dst_buf, int64_t size) override {
        (void)dst_buf;
        (void)size;
        return 0;
    }
    int64_t tellg() override {
        return 0;
    }
    int seekg(int64_t pos, int mode) override {
        (void)pos;
        (void)mode;
        return 0;
    }
    int64_t length() const override {
        return 0;
    }
};


TEST_F(AMp3AudioFormatReader, InitWithInputStream)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    Mp3AudioFormatReader reader(std::move(in_stream));
}

TEST_F(AMp3AudioFormatReader, ReturnsFalseIfOpenFailed)
{
    auto in_stream = std::unique_ptr<InputStream>(new FakeStream());

    Mp3AudioFormatReader reader(std::move(in_stream));

    ASSERT_FALSE(reader.isOpenOk());
}

TEST_F(AMp3AudioFormatReader, ReturnTrueIfOpenSuccessfully)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));

    Mp3AudioFormatReader reader(std::move(in_stream));

    ASSERT_TRUE(reader.isOpenOk());
}

TEST_F(AMp3AudioFormatReader, ReadReturnFalseIfOpenFailed)
{
    auto fake_stream = std::unique_ptr<InputStream>(new FakeStream());
    Mp3AudioFormatReader reader(std::move(fake_stream));
    ASSERT_FALSE(reader.isOpenOk());

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_FALSE(ret);
}
TEST_F(AMp3AudioFormatReader, CanReadSamples)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    Mp3AudioFormatReader reader(std::move(in_stream));

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_TRUE(ret);
}

TEST_F(AMp3AudioFormatReader, ReadWithStartOffsetOfFileWillChangePosition)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    Mp3AudioFormatReader reader(std::move(in_stream));

    const int offset = 5;
    reader.readSamples(dest, num_channels, 0, offset, n_read_samples);

    ASSERT_THAT(reader.getPosition(), Eq(offset));
}

//
//TEST_F(AMp3AudioFormatReader, ReadOnlyAvailableChannels)
//{
//    ifstream in_stream(test_file_name);
//    Mp3AudioFormatReader reader(std::move(in_stream));
//
//    const int num_dest_channels = 3;
//    reader.readSamples(dest, num_dest_channels, 0, 0, n_read_samples);
//
//    vector<float> ref(n_read_samples, fill_val);
//    vector<float> all_0(n_read_samples, 0.0f);
//    ASSERT_THAT(left_buffer, Pointwise(FloatNearPointwise(1e-3), ref));
//    ASSERT_THAT(right_buffer, Pointwise(FloatNearPointwise(1e-3), ref));
//    ASSERT_THAT(third_buffer, ContainerEq(all_0));
//}
//
//TEST_F(AMp3AudioFormatReader, ReadWithStartOfDest)
//{
//    ifstream in_stream(test_file_name);
//    Mp3AudioFormatReader reader(std::move(in_stream));
//
//    const int offset = 5;
//    reader.readSamples(dest, num_channels, offset, 0, n_read_samples);
//
//    vector<float> offset_data{left_buffer.begin(), left_buffer.begin() + offset};
//    vector<float> samples_data{left_buffer.begin() + offset, left_buffer.end()};
//    vector<float> ref(samples_data.size(), fill_val);
//    vector<float> all_0(offset, 0.0f);
//    ASSERT_THAT(offset_data, ContainerEq(all_0));
//    ASSERT_THAT(samples_data, Pointwise(FloatNearPointwise(1e-3), ref));
//}
//

