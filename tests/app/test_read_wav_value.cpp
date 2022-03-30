//
// Created by William on 3/29/22.
//
#include "../aa_testing_helper.h"
#include "ae_read_wav_value.h"
#include <gmock/gmock.h>
#include <iostream>

using namespace testing;
using namespace libaa;

class AReadWavValue : public Test {
public:
    ReadWavValue read_wav_value;
    DemoContext context;

    const size_t sample_rate = 44100;
    const size_t num_channels = 2;
    const size_t num_frames = sample_rate * 1;
    const size_t num_bits = 16;
    const float fill_val = 0.1f;

    const std::string test_file_name = "wave_audio_format_reader_test.wav";
};

TEST_F(AReadWavValue, CanOpenWavFileFromFile) {
    auto test_file = std::make_unique<ScopeWaveFile>(
        test_file_name, sample_rate, num_channels, num_frames, num_bits,
        fill_val);
    context.input_file_name = test_file_name;

    read_wav_value.process(context);

    ASSERT_THAT(context.in_sample_rate, Eq(sample_rate));
    ASSERT_THAT(context.in_num_channels, Eq(num_channels));
    ASSERT_THAT(context.in_num_frames, Eq(num_frames));
}

TEST_F(AReadWavValue, DecodeWholeWavFileToBuffer) {
    auto test_file = std::make_unique<ScopeWaveFile>(
        test_file_name, sample_rate, num_channels, num_frames, num_bits,
        fill_val);
    context.input_file_name = test_file_name;

    read_wav_value.process(context);

    ASSERT_THAT(context.in_samples.size(), Eq(num_channels));
    ASSERT_THAT(context.in_samples[0].size(), Eq(num_frames));
    ASSERT_THAT(context.in_samples[0][0], FloatEq(fill_val));
}