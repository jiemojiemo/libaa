
//
// Created by William.Hua on 2021/5/19.
//
#include "libaa/core/aa_audio_processor_parameter.h"
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
        param = make_unique<AudioProcessorParameter>(ParameterType::kFloat, param_id, test_name, default_value, min_plain_value, max_plain_value);
    }
    int param_id = 0;
    float default_value = 1;
    float min_plain_value = -10;
    float max_plain_value = 10;
    const std::string test_name = "test_param";
    std::vector<std::string> choices_strings = {"A", "B", "C"};

    std::unique_ptr<AudioProcessorParameter> param;
};


TEST_F(AAudioProcessorParameter, CanInitWithIDAndValues)
{
    AudioProcessorParameter p(ParameterType::kFloat, param_id, test_name, default_value, min_plain_value, max_plain_value);
}

TEST_F(AAudioProcessorParameter, CanGetParameterType)
{
    ASSERT_THAT(param->getParameterType(), Eq(ParameterType::kFloat));
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
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kFloat,param_id, "param_name", 100, -10, 10);

    ASSERT_THAT(param->getDefaultPlainValue(), Eq(param->getMinPlainValue()));
}

TEST_F(AAudioProcessorParameter, SetMinValueToMaxValueIfInitMinValueLargeThanMax)
{
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kFloat,param_id, "param_name", default_value, 100, 10);

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

TEST_F(AAudioProcessorParameter, CanGetParameterName)
{
    string name = param->getParameterName();

    ASSERT_THAT(name, Eq(test_name));
}

TEST_F(AAudioProcessorParameter, CanSetParameterName)
{
    string new_name = "xx";

    param->setParameterName(new_name);

    ASSERT_THAT(param->getParameterName(), Eq(new_name));
}

TEST_F(AAudioProcessorParameter, CanConstructWithChoiceStrings)
{
    int default_index = 0;
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kChoice, param_id, "choices",
                                                      default_index, 0, choices_strings.size(), choices_strings);

}

TEST_F(AAudioProcessorParameter, ChoiceStringCanConvertToNormalizedValueWithStep)
{
    int default_index = 1;
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kChoice, param_id, "choices",
                                                      default_index, 0, choices_strings.size(), choices_strings);

    float step = 1.0f / choices_strings.size();

    ASSERT_THAT(param->getNormalizedValue(), FloatEq(step * default_value));
}

TEST_F(AAudioProcessorParameter, CanConvertNormalizedValueToChoiceString)
{
    int default_index = 1;
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kChoice, param_id, "choices",
                                                      default_index, 0, choices_strings.size(), choices_strings);

    float norm_val = param->getNormalizedValue();
    ASSERT_THAT(param->convertNormalizedValueToChoiceString(norm_val),Eq(choices_strings[default_index]));
}

TEST_F(AAudioProcessorParameter, ConvertNormalizedValueReturnsEmptyIfChoiceStringIsEmpty)
{
    int default_index = 1;
    choices_strings = {};
    param = std::make_unique<AudioProcessorParameter>(ParameterType::kChoice, param_id, "choices",
                                                      default_index, 0, choices_strings.size(), choices_strings);

    float norm_val = 0.5;
    ASSERT_THAT(param->convertNormalizedValueToChoiceString(norm_val),Eq(""));
}