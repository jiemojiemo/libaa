//
// Created by william on 2019/12/22.
//

#include "libaa/fileio/aa_audio_file.h"
#include "libaa/fileio/aa_audio_decoder_factory.h"
#include <gmock/gmock.h>
#include <string>
#include <fstream>

using namespace libaa;
using namespace std;
using namespace testing;

void writeAFakeWave(const string& output_name, size_t num_frames=1024, size_t sample_rate=44100, size_t num_bits=16)
{
    AudioFile audiofile;
    vector<float> fake_left_data(num_frames, 1.0f);
    vector<float> fake_right_data = fake_left_data;

    audiofile.setNumChannles(2);
    audiofile.setSampleRate(sample_rate);
    audiofile.setNumBits(num_bits);

    audiofile.setChannelData(0, fake_left_data.data(), fake_left_data.size());
    audiofile.setChannelData(1, fake_right_data.data(), fake_right_data.size());

    audiofile.saveToWave(output_name);
}

class AAudioFile : public Test
{
public:
    void SetUp() override
    {
        writeAFakeWave(fake_wave_path);
    }

    AudioFile in_file;
    string fake_wave_path = "fake_wav.wav";

};

TEST_F(AAudioFile, LoadReturnsNotZeroIfFileNotExist)
{
    string no_exist_file = "xx.wav";

    ASSERT_THAT(in_file.load(no_exist_file), Not(0));
}

TEST_F(AAudioFile, WriteRawDataToWave)
{
    ifstream in(fake_wave_path);
    ASSERT_TRUE(in.good());
}

TEST_F(AAudioFile, LoadReturnsZeroIfFileIsValid)
{
    ASSERT_THAT(in_file.load(fake_wave_path), Eq(0));
}

TEST_F(AAudioFile, GetAudioInfoAfterLoadSuccessfully)
{
    in_file.load(fake_wave_path);

    ASSERT_THAT(in_file.getNumFrames(), 1024);
    ASSERT_THAT(in_file.getNumChannels(), 2);
    ASSERT_THAT(in_file.getSampleRate(), 44100);
    ASSERT_THAT(in_file.getNumBits(), 16);
}

class ADecoderFactory : public Test
{
public:
    void SetUp() override
    {
        support_format = AudioDecoderFactory::getSupportFormat();
    }
    vector<string> support_format;
};

TEST_F(ADecoderFactory, ReturnSupportedAudioFormat)
{
    ASSERT_THAT(support_format.size(), Gt(0));
}

TEST_F(ADecoderFactory, ReturnNullIfFileformatIsUnsupported)
{
    const string unsupported_filename = "abc.avi";

    auto decoder = AudioDecoderFactory::createDecoder(unsupported_filename);

    ASSERT_THAT(support_format, Not(Contains(unsupported_filename)));
    ASSERT_THAT(decoder, IsNull());
}