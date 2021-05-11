
//
// Created by William.Hua on 2021/5/11.
//
#include "libaa/fileio/aa_mp3_audio_format_writer.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include <gmock/gmock.h>
#include <fstream>
#include <vector>
using namespace testing;
using namespace std;
using namespace libaa;


class AMP3AudioFormatReader : public Test
{
public:
    void SetUp() override
    {
        out_stream = std::make_unique<FileOutputStream>();
        out_stream->open("test_wav.mp3");

        left_data.resize(num_samples, 0.2);
        right_data.resize(num_samples, 0.2);
        data_refer_to.resize(2);
        data_refer_to[0] = left_data.data();
        data_refer_to[1] = right_data.data();
        interleave_data.resize(num_channels * num_samples, 0.2);
    }

    std::unique_ptr<FileOutputStream> out_stream;
    const int sample_rate = 44100;
    const int num_channels = 2;
    const int num_bits = 32;

    const int num_samples = 64;
    vector<float> left_data;
    vector<float> right_data;
    vector<float> interleave_data;
    vector<const float*> data_refer_to;
};

TEST_F(AMP3AudioFormatReader, InitWithOuputStream)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);
}

TEST_F(AMP3AudioFormatReader, IsOpenReturnTrueIfInitSucessfully)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
}

TEST_F(AMP3AudioFormatReader, CanGetSampleRateIfOpenSucessfully)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.sample_rate, Eq(sample_rate));
}

TEST_F(AMP3AudioFormatReader, CanGetNumChannelsIfOpenSucessfully)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.num_channels, Eq(num_channels));
}

TEST_F(AMP3AudioFormatReader, NumBisIs32IfOpenSucessfully)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.isOpen());
    ASSERT_THAT(writer.num_bits, Eq(16));
}

TEST_F(AMP3AudioFormatReader, DISABLED_IsOpenReturnFalseIfOutputStreamOpenFailed)
{
    auto bad_stream = std::make_unique<FileOutputStream>("/bad/path");
    ASSERT_FALSE(bad_stream->isOpen());

    MP3FormatWriter writer(std::move(bad_stream), sample_rate, num_channels, num_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AMP3AudioFormatReader, OpenFailedIfSampleRateUnsupported)
{
    const int unsupported_sample_rate = -1;

    MP3FormatWriter writer(std::move(out_stream), unsupported_sample_rate, num_channels, num_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AMP3AudioFormatReader, OpenFailedIsNumChannelUnsupported)
{
    const int invalid_channels = 0;

    MP3FormatWriter writer(std::move(out_stream), sample_rate, invalid_channels, num_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AMP3AudioFormatReader, OpenFailedIsNumBitsUnsupported)
{
    const int invalid_bits = -1;

    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, invalid_bits);

    ASSERT_FALSE(writer.isOpen());
}

TEST_F(AMP3AudioFormatReader, WritePlanarFaildIfOpenFailed)
{
    const int invalid_bits = -1;

    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, invalid_bits);
    ASSERT_FALSE(writer.isOpen());

    ASSERT_FALSE(writer.writePlanar(data_refer_to.data(), num_samples));
}

TEST_F(AMP3AudioFormatReader, WriteInterleaveFaildIfOpenFailed)
{
    const int invalid_bits = -1;

    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, invalid_bits);
    ASSERT_FALSE(writer.isOpen());

    ASSERT_FALSE(writer.writeInterleave(interleave_data.data(), num_samples));
}

TEST_F(AMP3AudioFormatReader, CanWritePlanar)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.writePlanar(data_refer_to.data(), num_samples));
}

TEST_F(AMP3AudioFormatReader, CanWriteInterleave)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);

    ASSERT_TRUE(writer.writeInterleave(interleave_data.data(), num_samples));
}

TEST_F(AMP3AudioFormatReader, CloseWillResetAudioConfig)
{
    MP3FormatWriter writer(std::move(out_stream), sample_rate, num_channels, num_bits);
    writer.close();

    ASSERT_THAT(writer.sample_rate, Eq(-1));
    ASSERT_THAT(writer.num_channels, Eq(-1));
    ASSERT_THAT(writer.num_bits, Eq(-1));
}