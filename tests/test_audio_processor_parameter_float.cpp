
//
// Created by William.Hua on 2021/5/19.
//
#include "libaa/core/aa_audio_processor_parameter_float.h"
#include <gmock/gmock.h>
#include <memory>
using namespace std;
using namespace testing;
using namespace libaa;

class AAudioProcessorParameter : public Test
{
public:
    void SetUp() override
    {
        param = make_unique<AudioProcessorParameterFloat>(param_id, default_value, min_plain_value, max_plain_value);
    }
    int param_id = 0;
    float default_value = 1;
    float min_plain_value = -10;
    float max_plain_value = 10;

    std::unique_ptr<AudioProcessorParameterFloat> param;
};


TEST_F(AAudioProcessorParameter, CanInitWithIDAndValues)
{
    AudioProcessorParameterFloat p(param_id, default_value, min_plain_value, max_plain_value);
}

TEST_F(AAudioProcessorParameter, CanGetPlainValueAfterInit)
{
    ASSERT_THAT(param->getDefaultPlainValue(), Eq(default_value));
}

TEST_F(AAudioProcessorParameter, CanGetParamterID)
{
    ASSERT_THAT(param->getParameterID(), Eq(param_id));
}

TEST_F(AAudioProcessorParameter, CanGetParamterRange)
{
    ASSERT_THAT(param->getMinPlainValue(), Eq(min_plain_value));
    ASSERT_THAT(param->getMaxPlainValue(), Eq(max_plain_value));
}

TEST_F(AAudioProcessorParameter, DefaultValueSetToMinValueIfNotInRange)
{
    param = std::make_unique<AudioProcessorParameterFloat>(param_id, 100, -10, 10);

    ASSERT_THAT(param->getDefaultPlainValue(), Eq(param->getMinPlainValue()));
}

TEST_F(AAudioProcessorParameter, SetMinValueToMaxValueIfInitMinValueLargeThanMax)
{
    param = std::make_unique<AudioProcessorParameterFloat>(param_id, default_value, 100, 10);

    ASSERT_THAT(param->getMinPlainValue(), Eq(max_plain_value));
}

TEST_F(AAudioProcessorParameter, CanGetNormalizedValueFromPlainValue)
{
    float plain_value = param->getMinPlainValue() + 2;
    float normalized_value = (plain_value - param->getMinPlainValue()) / (max_plain_value - min_plain_value);

    ASSERT_THAT(param->convertPlainValueToNormalizedValue(plain_value), Eq(normalized_value));
}

TEST_F(AAudioProcessorParameter, GetNormalizedValueClippedIfOutOfMinValue)
{
    float plain_value = param->getMinPlainValue() - 1;

    ASSERT_THAT(plain_value, Lt(param->getMinPlainValue()));
    ASSERT_THAT(param->convertPlainValueToNormalizedValue(plain_value), Eq(0));
}

TEST_F(AAudioProcessorParameter, GetNormalizedValueClippedIfOutOfMaxValue)
{
    float plain_value = param->getMaxPlainValue() + 1;

    ASSERT_THAT(plain_value, Gt(param->getMaxPlainValue()));
    ASSERT_THAT(param->convertPlainValueToNormalizedValue(plain_value), Eq(1));
}

TEST_F(AAudioProcessorParameter, CanGetPlainValueFromNormalizedValue)
{
    float normalized_v = 0.5;
    float plain_value = (max_plain_value - min_plain_value) * normalized_v + min_plain_value;

    ASSERT_THAT(param->convertNormalizedValueToPlainValue(normalized_v), Eq(plain_value));
}

TEST_F(AAudioProcessorParameter, GetPlainValueClippedFromIfLessThan0)
{
    float normalized_v = -0.1;

    ASSERT_THAT(param->convertNormalizedValueToPlainValue(normalized_v), Eq(min_plain_value));
}

TEST_F(AAudioProcessorParameter, GetPlainValueClippedFromIfLargeThan1)
{
    float normalized_v = 1.1;

    ASSERT_THAT(param->convertNormalizedValueToPlainValue(normalized_v), Eq(max_plain_value));
}

TEST_F(AAudioProcessorParameter, CurrentPlainValueIsEqDefaultPlainValueWhenInit)
{
    ASSERT_THAT(param->getPlainValue(), Eq(param->getDefaultPlainValue()));
}

TEST_F(AAudioProcessorParameter, CanSetPlainValue)
{
    float val = min_plain_value + 2;

    param->setPlainValue(val);

    ASSERT_THAT(param->getPlainValue(), Eq(val));
}

TEST_F(AAudioProcessorParameter, SetPlainValueWillClipIfOutOfMinRange)
{
    float val = min_plain_value - 2;

    param->setPlainValue(val);

    ASSERT_THAT(param->getPlainValue(), Eq(param->getMinPlainValue()));
}

TEST_F(AAudioProcessorParameter, SetPlainValueWillClipIfOutOfMaxRange)
{
    float val = max_plain_value + 2;

    param->setPlainValue(val);

    ASSERT_THAT(param->getPlainValue(), Eq(param->getMaxPlainValue()));
}

TEST_F(AAudioProcessorParameter, CanGetCurrentNormalizedValue)
{
    ASSERT_THAT(param->getNormalizedValue(),
                Eq(param->convertPlainValueToNormalizedValue(param->getDefaultPlainValue())));
}

TEST_F(AAudioProcessorParameter, SetNormalizedValueWillClipIfOutOfMaxRange)
{
    param->setNormalizedValue(1.1);

    ASSERT_THAT(param->getNormalizedValue(), Eq(1.0));
}

TEST_F(AAudioProcessorParameter, SetNormalizedValueWillClipIfOutOfMinRange)
{
    param->setNormalizedValue(-0.1);

    ASSERT_THAT(param->getNormalizedValue(), Eq(0));
}

TEST_F(AAudioProcessorParameter, CanGetPlainValueString)
{
    ASSERT_THAT(param->getPlainValueString(), Eq("1.000000"));
}

TEST_F(AAudioProcessorParameter, CanSetPlainValueString)
{
    string str = "10.0";

    param->setPlainValue(str);

    ASSERT_THAT(param->getPlainValue(), Eq(std::stof(str)));
}

TEST_F(AAudioProcessorParameter, CanSetNormalizedValueString)
{
    string str = "0.5";

    param->setNormalizedValue(str);

    ASSERT_THAT(param->getNormalizedValue(), Eq(std::stof(str)));
}
