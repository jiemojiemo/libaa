//
// Created by user on 11/29/22.
//
#include <gmock/gmock.h>

#include "libaa/dsp/aa_deinterleave_utils.h"

using namespace testing;
using namespace libaa::DeinterleaveUtils;

class ADeinterleaveUtils : public Test {
public:
};

TEST_F(ADeinterleaveUtils, DeinterleaveToPlanar) {
    std::vector<int> L{0, 2, 4, 6};
    std::vector<int> R{1, 3, 5, 7};
    std::vector<int *> data_refer_to{L.data(), R.data()};
    std::vector<int> interleave_buf(L.size() * 2);
    planarToInterleave(data_refer_to.data(), data_refer_to.size(), L.size(), interleave_buf.data());

    std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7};
    ASSERT_THAT(interleave_buf, Eq(expected));
}