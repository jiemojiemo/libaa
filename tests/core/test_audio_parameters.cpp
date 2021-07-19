
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
    params.addFloatParameter(0, 1.0, 0, 2.0);

    ASSERT_THAT(params.size(), Eq(1));
}

TEST_F(AParameters, CanAddFloatParameter)
{
    params.addFloatParameter(0, 1.0, 0, 2.0);

    auto* p = params.get(0);
    AudioProcessorParameter expected{ParameterType::kFloat, 0, 1.0, 0, 2.0};

    ASSERT_THAT(*p, Eq(expected));
}

TEST_F(AParameters, AddParamterWithExistedIsWillOverwriteBeforeOne)
{
    int param_id = 0;
    params.addFloatParameter(param_id, 1.0, 0, 2.0);
    params.addFloatParameter(param_id, 2.0, 0, 2.0);

    AudioProcessorParameter expected{ParameterType::kFloat, param_id, 2.0, 0, 2.0};
    ASSERT_THAT(params.size(), Eq(1));
    ASSERT_THAT(params.get(param_id), Pointee(expected));
}

TEST_F(AParameters, GetNullptrIfIDNotExist)
{
    auto* p = params.get(2);

    ASSERT_THAT(p, IsNull());
}

