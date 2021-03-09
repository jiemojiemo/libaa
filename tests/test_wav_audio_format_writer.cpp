
//
// Created by William.Hua on 2021/3/8.
//
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include <gmock/gmock.h>
#include <fstream>
#include <vector>
using namespace testing;
using namespace std;
using namespace libaa;

class AWaveFormatWriter : public Test
{
public:
    void SetUp() override
    {
        out_stream.open("test_wav.wav");

        left_data.resize(num_samples, 0.2);
        right_data.resize(num_samples, 0.2);
        data_refer_to.resize(2);
        data_refer_to[0] = left_data.data();
        data_refer_to[1] = right_data.data();
        interleave_data.resize(num_channels * num_samples, 0.2);
    }

    std::ofstream out_stream;
    const int sample_rate = 44100;
    const int num_channels = 2;
    const int num_bits = 32;

    const int num_samples = 64;
    vector<float> left_data;
    vector<float> right_data;
    vector<float> interleave_data;
    vector<const float*> data_refer_to;
};

TEST_F(AWaveFormatWriter, InitWithOuputStream)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);
}

TEST_F(AWaveFormatWriter, IsOpenReturnTrueIfInitSucessfully)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
}

TEST_F(AWaveFormatWriter, CanGetSampleRateIfOpenSucessfully)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.sample_rate, Eq(sample_rate));
}

TEST_F(AWaveFormatWriter, CanGetNumChannelsIfOpenSucessfully)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.num_channels, Eq(num_channels));
}

TEST_F(AWaveFormatWriter, NumBisIs32IfOpenSucessfully)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.num_bits, Eq(32));
}

TEST_F(AWaveFormatWriter, IsOpenReturnFalseIfOutputStreamOpenFailed)
{
    std::ofstream bad_stream;

    WavFormatWriter writer(bad_stream, sample_rate, num_channels, num_bits);

    ASSERT_FALSE(bad_stream.is_open());
    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AWaveFormatWriter, OpenFailedIfSampleRateUnsupported)
{
    const int unsupported_sample_rate = -1;

    WavFormatWriter writer(out_stream, unsupported_sample_rate, num_channels, num_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AWaveFormatWriter, OpenFailedIsNumChannelUnsupported)
{
    const int invalid_channels = 0;

    WavFormatWriter writer(out_stream, sample_rate, invalid_channels, num_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AWaveFormatWriter, OpenFailedIsNumBitsUnsupported)
{
    const int invalid_bits = -1;

    WavFormatWriter writer(out_stream, sample_rate, num_channels, invalid_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AWaveFormatWriter, WritePlanarFaildIfOpenFailed)
{
    const int invalid_bits = -1;

    WavFormatWriter writer(out_stream, sample_rate, num_channels, invalid_bits);
    ASSERT_FALSE(writer.isOpen());

    ASSERT_FALSE(writer.writePlanar(data_refer_to.data(), num_samples));
}

TEST_F(AWaveFormatWriter, WriteInterleaveFaildIfOpenFailed)
{
    const int invalid_bits = -1;

    WavFormatWriter writer(out_stream, sample_rate, num_channels, invalid_bits);
    ASSERT_FALSE(writer.isOpen());

    ASSERT_FALSE(writer.writeInterleave(interleave_data.data(), num_samples));
}

TEST_F(AWaveFormatWriter, CanWritePlanar)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.writePlanar(data_refer_to.data(), num_samples));
}

TEST_F(AWaveFormatWriter, CanWriteInterleave)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.writeInterleave(interleave_data.data(), num_samples));
}

TEST_F(AWaveFormatWriter, CloseWillResetAudioConfig)
{
    WavFormatWriter writer(out_stream, sample_rate, num_channels, num_bits);
    writer.close();

    ASSERT_THAT(writer.sample_rate, Eq(-1));
    ASSERT_THAT(writer.num_channels, Eq(-1));
    ASSERT_THAT(writer.num_bits, Eq(-1));
}