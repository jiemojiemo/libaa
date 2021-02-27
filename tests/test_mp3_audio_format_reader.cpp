
//
// Created by William.Hua on 2021/2/9.
//
#include "libaa/fileio/aa_mp3_audio_format_reader.h"

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


TEST_F(AMp3AudioFormatReader, DISABLED_InitWithInputStream)
{
    ifstream in_stream(test_file_name);
    Mp3AudioFormatReader reader(in_stream);
}

TEST_F(AMp3AudioFormatReader, DISABLED_ReturnsFalseIfOpenFailed)
{
    stringstream fake_stream;
    fake_stream << "";

    Mp3AudioFormatReader reader(fake_stream);

    ASSERT_FALSE(reader.isOpenOk());
}

TEST_F(AMp3AudioFormatReader, DISABLED_ReturnTrueIfOpenSuccessfully)
{
    fstream in_stream(test_file_name, std::ios::in);
    ASSERT_TRUE(in_stream.is_open());

    Mp3AudioFormatReader reader(in_stream);

    ASSERT_TRUE(reader.isOpenOk());
}

TEST_F(AMp3AudioFormatReader, DISABLED_ReadReturnFalseIfOpenFailed)
{
    stringstream fake_stream;
    fake_stream << "";
    Mp3AudioFormatReader reader(fake_stream);
    ASSERT_FALSE(reader.isOpenOk());

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_FALSE(ret);
}
TEST_F(AMp3AudioFormatReader, DISABLED_CanReadSamples)
{
    ifstream in_stream(test_file_name);
    Mp3AudioFormatReader reader(in_stream);

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_TRUE(ret);
}

TEST_F(AMp3AudioFormatReader, DISABLED_ReadWithStartOffsetOfFileWillChangePosition)
{
    ifstream in_stream(test_file_name);
    Mp3AudioFormatReader reader(in_stream);

    const int offset = 5;
    reader.readSamples(dest, num_channels, 0, offset, n_read_samples);

    ASSERT_THAT(reader.getPosition(), Eq(offset));
}

//
//TEST_F(AMp3AudioFormatReader, ReadOnlyAvailableChannels)
//{
//    ifstream in_stream(test_file_name);
//    Mp3AudioFormatReader reader(in_stream);
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
//    Mp3AudioFormatReader reader(in_stream);
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

