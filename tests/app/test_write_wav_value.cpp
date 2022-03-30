//
// Created by William on 3/29/22.
//
#include "ae_write_wav_value.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class AWriteWavValue : public Test {
public:
    DemoContext context;
    WriteWavValue value;
    ReadWavValue read_wav_value;

    const size_t sample_rate = 44100;
    const size_t num_channels = 2;
    const size_t num_frames = sample_rate * 1;
    const size_t num_bits = 16;
    const float fill_val = 0.1f;
};

TEST_F(AWriteWavValue, CanWriteSamplesToLocalFile) {
    context.output_dir = "./";
    context.output_file_name = "write_wav_test.wav";
    context.in_samples.resize(num_channels);
    context.in_sample_rate = sample_rate;
    context.in_num_channels = num_channels;
    context.in_num_frames = num_frames;
    for (auto &c : context.in_samples) {
        c.resize(num_frames, fill_val);
    }

    value.process(context);
    context.in_samples.clear();
    context.in_sample_rate = 0;

    // try to read from local file
    context.input_file_name = context.output_file_name;
    read_wav_value.process(context);

    ASSERT_THAT(context.in_sample_rate, Eq(sample_rate));
    ASSERT_THAT(context.in_samples.size(), Eq(num_channels));
    ASSERT_THAT(context.in_samples[0][0], Eq(fill_val));

    // clear
    std::string save_path = context.output_dir + context.output_file_name;
    remove(save_path.c_str());
}

TEST_F(AWriteWavValue, CanGetFullSavePathAfterWrite) {
    context.output_dir = "./";
    context.output_file_name = "write_wav_test.wav";
    context.in_samples.resize(num_channels);
    for (auto &c : context.in_samples) {
        c.resize(num_frames, fill_val);
    }

    value.process(context);

    // clear
    std::string save_path = context.output_dir + context.output_file_name;
    ASSERT_THAT(context.output_full_path, Eq(save_path));
}