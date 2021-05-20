//
// Created by william on 2020/2/20.
//

#include "libaa/audio_basics/aa_delay_line.h"
#include "libaa/audio_basics/aa_delay_line_array.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;


class ADelayLine : public Test
{
public:
    DelayLine<float> dline;
};

TEST_F(ADelayLine, SizeIsZeroAfterCreate)
{
    ASSERT_THAT(dline.size(), Eq(0));
}

TEST_F(ADelayLine, SizeChangedAfterResize)
{
    int target_size = 10;
    dline.resize(target_size);

    ASSERT_THAT(dline.size(), Eq(target_size));
}

TEST_F(ADelayLine, GetNewPushedValueFromBack)
{
    int target_size = 10;
    dline.resize(target_size);

    float x = 1.0;
    dline.push(x);

    ASSERT_THAT(dline.back(), FloatEq(x));
}

TEST_F(ADelayLine, GetNewPushedValueIfDelayIsZero)
{
    int target_size = 10;
    dline.resize(target_size);

    float x = 1.0;
    dline.push(x);

    size_t delay_in_sample = 0;
    ASSERT_THAT(dline.get(delay_in_sample), FloatEq(x));
}

TEST_F(ADelayLine, GetDelayedValue)
{
    int target_size = 10;
    dline.resize(target_size);

    for(int i = 0; i < target_size; ++i){ dline.push(float(i)); }

    size_t delay_in_sample = 5;

    ASSERT_THAT(dline.get(delay_in_sample), FloatEq(target_size - delay_in_sample - 1));
}

TEST_F(ADelayLine, SetAllValueToZeroAfterClear)
{
    int target_size = 10;
    dline.resize(target_size);
    dline.push(1.0);

    dline.clear();

    ASSERT_THAT(dline.back(), FloatEq(0.0f));
}

TEST_F(ADelayLine, RetrunsInterpolatonValueIfDelayIsFloat)
{
    int target_size = 10;
    dline.resize(target_size);
    for(int i = 0; i < target_size; ++i){ dline.push(float(i)); }

    float delay = 2.5;

    ASSERT_THAT(dline.getInterpolation(delay), FloatEq(6.5f));
}



