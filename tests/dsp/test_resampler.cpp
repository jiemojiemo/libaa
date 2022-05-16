//
// Created by user on 5/16/22.
//

#include "libaa/dsp/aa_resampler.h"
#include <gmock/gmock.h>
#include <vector>
using namespace testing;
using namespace libaa;
using namespace std;
class AResampler : public Test {
public:
    Resampler resampler;
    float sample_rate = 44100;
    int num_channels = 2;
    int max_block_size = 10;
};

TEST_F(AResampler, PrepareWillCreateResampleImpl) {
    resampler.prepare(sample_rate, num_channels, max_block_size);

    ASSERT_THAT(resampler.getResampleImpl(), NotNull());
}

TEST_F(AResampler, CanGetNumberChannels) {
    resampler.prepare(sample_rate, num_channels, max_block_size);

    ASSERT_THAT(resampler.getNumberChannels(), Eq(num_channels));
}

TEST_F(AResampler, CanProcessPlannarData) {
    vector<float> left_data(2 * max_block_size, 0.5);
    vector<float> right_data(2 * max_block_size, 0.6);
    vector<float *> data_refer_to{left_data.data(), right_data.data()};

    resampler.prepare(sample_rate, num_channels, max_block_size);
    resampler.resample(data_refer_to.data(), 5, data_refer_to.data(),
                       2 * max_block_size, 1.5, false);
}

TEST_F(AResampler, CanProcessInterleaveData) {
    float ratio = 1.5;
    vector<float> in_data(max_block_size * num_channels, 0.6);
    vector<float> out_data(ceil(max_block_size * num_channels * ratio));

    resampler.prepare(sample_rate, num_channels, max_block_size);
    resampler.resampleInterleaved(in_data.data(), 5, out_data.data(),
                                  2 * max_block_size, 1.5, false);
}