
//
// Created by William.Hua on 2021/7/18.
//

#include <gmock/gmock.h>

#include "libaa/core/aa_audio_processor_parameters.h"
using namespace testing;
using namespace libaa;

class AParameters : public Test
{
public:
    Parameters params;
};

TEST_F(AParameters, DefaultContructWithEmpty)
{
    Parameters params;

    ASSERT_THAT(params.size(), Eq(0));
}

TEST_F(AParameters, AddParametersIncreaseTheSize)
{
    params.pushFloatParameter(1.0, 0, 2.0);

    ASSERT_THAT(params.size(), Eq(1));
}

TEST_F(AParameters, CanAddFloatParameter)
{
    params.pushFloatParameter(1.0, 0, 2.0);

    AudioProcessorParameter& p = params.get(0);
    AudioProcessorParameter expected{ParameterType::kFloat, 0, 1.0, 0, 2.0};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, ThrowIfParameterIndexNotExist)
{
    EXPECT_ANY_THROW(params.get(2));
}

