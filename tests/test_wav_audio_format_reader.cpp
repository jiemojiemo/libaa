
//
// Created by William.Hua on 2021/2/9.
//
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_file_stream.h"
#include "aa_test_helper.h"
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
using namespace testing;
using namespace libaa;
using namespace std;


class AWaveAudioFormatReader : public Test
{
public:
    void SetUp() override
    {
        test_file = std::make_unique<ScopeWaveFile>(test_file_name,sample_rate,
                                                    num_channels,
                                                    num_frames,
                                                    num_bits,
                                                    fill_val);

        left_buffer.resize(n_read_samples);
        right_buffer.resize(n_read_samples);
        third_buffer.resize(n_read_samples);
        dest[0] = left_buffer.data();
        dest[1] = right_buffer.data();
        dest[2] = third_buffer.data();

        in_stream = std::unique_ptr<InputStream>(new FileStream(test_file_name));
    }

    void TearDown() override
    {
        test_file = nullptr;
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

    const std::string test_file_name="wave_audio_format_reader_test.wav";
    std::unique_ptr<ScopeWaveFile> test_file;
    std::unique_ptr<InputStream> in_stream;
};

class FakeStream : public InputStream
{
public:
    ~FakeStream() override = default;
    int64_t read(uint8_t *dst_buf, int64_t size) override {
        return 0;
    }
    int64_t tellg() override {
        return 0;
    }
    int seekg(int64_t pos, int mode) override {
        return 0;
    }
    int64_t length() const override {
        return 0;
    }
};



TEST_F(AWaveAudioFormatReader, InitWithInputStream)
{
    WaveAudioFormatReader reader(std::move(in_stream));
}

TEST_F(AWaveAudioFormatReader, ReturnsFalseIfOpenFailed)
{
    auto fake_stream  =std::unique_ptr<InputStream>(new FakeStream);

    WaveAudioFormatReader reader(std::move(fake_stream));

    ASSERT_FALSE(reader.isOpenOk());
}

TEST_F(AWaveAudioFormatReader, ReturnTrueIfOpenSuccessfully)
{
    WaveAudioFormatReader reader(std::move(in_stream));

    ASSERT_TRUE(reader.isOpenOk());
}

TEST_F(AWaveAudioFormatReader, ReadReturnFalseIfOpenFailed)
{
    auto fake_stream  =std::unique_ptr<InputStream>(new FakeStream);

    WaveAudioFormatReader reader(std::move(fake_stream));
    ASSERT_FALSE(reader.isOpenOk());

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_FALSE(ret);
}

TEST_F(AWaveAudioFormatReader, CanReadSamples)
{
    WaveAudioFormatReader reader(std::move(in_stream));

    auto ret = reader.readSamples(dest, num_channels, 0, 0, n_read_samples);

    ASSERT_TRUE(ret);
    vector<float> ref(n_read_samples, fill_val);
    ASSERT_THAT(left_buffer, Pointwise(FloatNearPointwise(1e-3), ref));
}

TEST_F(AWaveAudioFormatReader, ReadOnlyAvailableChannels)
{
    WaveAudioFormatReader reader(std::move(in_stream));

    const int num_dest_channels = 3;
    reader.readSamples(dest, num_dest_channels, 0, 0, n_read_samples);

    vector<float> ref(n_read_samples, fill_val);
    vector<float> all_0(n_read_samples, 0.0f);
    ASSERT_THAT(left_buffer, Pointwise(FloatNearPointwise(1e-3), ref));
    ASSERT_THAT(right_buffer, Pointwise(FloatNearPointwise(1e-3), ref));
    ASSERT_THAT(third_buffer, ContainerEq(all_0));
}

TEST_F(AWaveAudioFormatReader, ReadWithStartOfDest)
{
    WaveAudioFormatReader reader(std::move(in_stream));

    const int offset = 5;
    reader.readSamples(dest, num_channels, offset, 0, n_read_samples);

    vector<float> offset_data{left_buffer.begin(), left_buffer.begin() + offset};
    vector<float> samples_data{left_buffer.begin() + offset, left_buffer.end()};
    vector<float> ref(samples_data.size(), fill_val);
    vector<float> all_0(offset, 0.0f);
    ASSERT_THAT(offset_data, ContainerEq(all_0));
    ASSERT_THAT(samples_data, Pointwise(FloatNearPointwise(1e-3), ref));
}

TEST_F(AWaveAudioFormatReader, ReadWithStartOffsetOfFileWillChangePosition)
{
    WaveAudioFormatReader reader(std::move(in_stream));

    const int offset = 5;
    reader.readSamples(dest, num_channels, 0, offset, n_read_samples);

    ASSERT_THAT(reader.getPosition(), Eq(offset));
}
