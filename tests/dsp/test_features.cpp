//
// Created by william on 2019/12/24.
//

#include "libaa/dsp/aa_fft.h"
#include "libaa/dsp/aa_stft.h"
#include "libaa/dsp/aa_window.h"
#include "libaa/dsp/aa_mfcc.h"
#include "../aa_test_helper.h"
#include <gmock/gmock.h>
#include <Eigen/Core>
#include <vector>
#include <iostream>

using namespace testing;
using namespace Eigen;
using namespace std;
using namespace libaa;

class ASTFT : public Test
{
public:
    void SetUp() override
    {
        fake_data.resize(data_length);
        std::fill(fake_data.begin(), fake_data.end(), 1.0);

        opts.hop_size = hop_size;
        opts.win_size = win_size;
    }
    const int hop_size = 6;
    const int win_size = 8;
    const int data_length = 40;

    Options opts;
    vector<float> fake_data;
    Eigen::ArrayXXcf stft_result;

};

TEST_F(ASTFT, NumberFramesEqualsSizeDivideHopSize)
{
    stft_result = STFT::stft(fake_data.data(), fake_data.size(), opts);

    ASSERT_THAT(stft_result.rows(), Eq(win_size/2 + 1));
    ASSERT_THAT(stft_result.cols(), Eq(data_length / hop_size));
}

TEST(MFCCTest, OutputSize)
{
    const int data_length = 2048;
    const int sample_rate = 44100;
    ArrayXf input_data = ArrayXf::Random(data_length);

    Options opts;
    opts.n_mels = 88;
    opts.n_mfcc = 22;
    opts.hop_size = 128;

    ArrayXXf mfccs = MFCC::mfcc(input_data.data(), input_data.size(), sample_rate, opts);

    EXPECT_THAT(mfccs.rows(), Eq(opts.n_mfcc));
    EXPECT_THAT(mfccs.cols(), Eq(data_length / opts.hop_size));
}
