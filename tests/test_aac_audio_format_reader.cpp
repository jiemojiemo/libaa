
//
// Created by William.Hua on 2021/3/22.
//
#include "libaa/fileio/aa_aac_audio_format_reader.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include <gmock/gmock.h>
#include <vector>
using namespace std;
using namespace testing;
using namespace libaa;


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

class AAACAudioFormatReader : public Test
{
public:
    void SetUp() override
    {
        in_stream = std::make_unique<FileStream>(test_file_name);
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

    const std::string test_file_name="../../res/sound/sample1.aac";
    std::unique_ptr<InputStream> in_stream;
};

TEST_F(AAACAudioFormatReader, InitWithInputStream)
{
    AACAudioFormatReader read(std::move(in_stream));
}

TEST_F(AAACAudioFormatReader, ReturnsFalseIfOpenFailed)
{
    auto fake_stream = std::unique_ptr<InputStream>(new FakeStream());

    AACAudioFormatReader reader(std::move(fake_stream));

    ASSERT_FALSE(reader.isOpenOk());
}

TEST_F(AAACAudioFormatReader, ReturnTrueIfOpenSuccessfully)
{
    AACAudioFormatReader reader(std::move(in_stream));

    ASSERT_TRUE(reader.isOpenOk());
}

TEST_F(AAACAudioFormatReader, CanGetAudioInformationIfOpenSuccessfully)
{
    AACAudioFormatReader reader(std::move(in_stream));

    ASSERT_TRUE(reader.isOpenOk());
    ASSERT_THAT(reader.num_channels, Eq(2));
    ASSERT_THAT(reader.sample_rate, Eq(44100));
    ASSERT_THAT(reader.num_bits, Eq(16));
    ASSERT_THAT(reader.length_in_samples, Gt(0));
}

TEST_F(AAACAudioFormatReader, CanReadSamples)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    AACAudioFormatReader reader(std::move(in_stream));

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_TRUE(ret);
}

TEST_F(AAACAudioFormatReader, ReadWithStartOffsetOfFileWillChangePosition)
{
    auto in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    AACAudioFormatReader reader(std::move(in_stream));

    const int offset = 5;
    reader.readSamples(dest, num_channels, 0, offset, n_read_samples);

    ASSERT_THAT(reader.getPosition(), Eq(offset));
}