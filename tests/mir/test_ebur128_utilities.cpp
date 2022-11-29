//
// Created by user on 11/29/22.
//
#include <gmock/gmock.h>

#include "libaa/mir/aa_ebur128_utilities.h"

using namespace testing;
using namespace libaa::MIR;

class AEbur128Util : public Test {
public:
    int num_channels = 2;
    int sample_rate = 44100;
    int max_block_size = 1024;

    Ebur128Utilities u;
};

TEST_F(AEbur128Util, CanInitWithSampleRateAndChannels) {
    ASSERT_THAT(u.st, IsNull());

    u.prepare(num_channels, sample_rate, max_block_size);

    ASSERT_THAT(u.st, NotNull());
}

TEST_F(AEbur128Util, CleanUpWillDestroyInternalState) {
    u.prepare(num_channels, sample_rate, max_block_size);
    u.cleanup();

    ASSERT_THAT(u.st, IsNull());
}

TEST_F(AEbur128Util, CanProcessFrameBlock) {
    int num_frames = max_block_size;
    std::vector<float> r(num_frames, 0.2f);
    std::vector<float *> data_refer_to{r.data(), r.data()};

    u.prepare(num_channels, sample_rate, max_block_size);
    bool ok = u.process(data_refer_to.data(), num_channels, num_frames);

    ASSERT_TRUE(ok);
}