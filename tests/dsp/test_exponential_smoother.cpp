
//
// Created by William.Hua on 2021/8/6.
//

#include <gmock/gmock.h>
#include "libaa/dsp/aa_exponential_smoother.h"
using namespace testing;
using namespace libaa;

class AExponentialSmoother : public Test
{
public:
    float alpha = 0.5;
};


TEST_F(AExponentialSmoother, ConstructWithAlpha)
{
    auto smoother = ExponentialSmoother{alpha};
}

TEST_F(AExponentialSmoother, CanGetAlpha)
{
    auto smoother = ExponentialSmoother{alpha};

    ASSERT_THAT(smoother.getAlpha(), FloatEq(alpha));
}

TEST_F(AExponentialSmoother, CanSetAlpha)
{
    auto smoother = ExponentialSmoother{alpha};

    float new_alpha = 0.6;
    smoother.setAlpha(new_alpha);

    ASSERT_THAT(smoother.getAlpha(), FloatEq(new_alpha));
}

TEST_F(AExponentialSmoother, CanSmoothProcess)
{
    auto smoother = ExponentialSmoother{alpha};

    float in_val = 1.0f;
    float expected = 0.0f;

    expected = in_val * alpha + (1-alpha) * expected;
    ASSERT_THAT(smoother.process(in_val), Eq(expected));

    expected = in_val * alpha + (1-alpha) * expected;
    ASSERT_THAT(smoother.process(in_val), Eq(expected));
}

