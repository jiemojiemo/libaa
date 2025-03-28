
//
// Created by William.Hua on 2021/7/18.
//

#include <gmock/gmock.h>

#include "libaa/core/aa_audio_processor_parameters.h"
using namespace testing;
using namespace libaa;

class AParameters : public Test {
public:
    AudioProcessorParameters params;
};

TEST_F(AParameters, DefaultContructWithEmpty) {
    AudioProcessorParameters params;

    ASSERT_THAT(params.size(), Eq(0));
}

TEST_F(AParameters, AddParametersIncreaseTheSize) {
    params.pushFloatParameter("param_name", 1.0, 0, 2.0);

    ASSERT_THAT(params.size(), Eq(1));
}

TEST_F(AParameters, AddParametersIncreaseTheParameterIndex) {
    params.pushFloatParameter("param_name", 1.0, 0, 2.0);
    params.pushFloatParameter("param_name", 1.0, 0, 2.0);

    AudioProcessorParameter &p = params.get(1);
    AudioProcessorParameter expected{
        ParameterType::kFloat, 1, "param_name", 1.0, 0, 2.0};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, CanAddFloatParameter) {
    params.pushFloatParameter("param_name", 1.0, 0, 2.0);

    AudioProcessorParameter &p = params.get(0);
    AudioProcessorParameter expected{
        ParameterType::kFloat, 0, "param_name", 1.0, 0, 2.0};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, ThrowIfParameterIndexNotExist) {
    EXPECT_ANY_THROW(params.get(2));
}

TEST_F(AParameters, CanAddChoiceParameter) {
    std::vector<std::string> choices{"A", "B"};
    params.pushChoiceParameter("choice", 0, choices);

    AudioProcessorParameter &p = params.get(0);
    AudioProcessorParameter expected{
        ParameterType::kChoice, 0, "choice", 0, 0,
        float(choices.size() - 1), choices};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, CanAddBoolParameter) {
    params.pushBoolParameter("bool", true);

    AudioProcessorParameter &p = params.get(0);
    AudioProcessorParameter expected{
        ParameterType::kFloat, 0, "bool", 1.0f, 0, 1.0f};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, CanFindParameterByNameAndReturnsIndex) {
    params.pushFloatParameter("A", 1.0, 0, 2.0);
    params.pushBoolParameter("B", true);

    ASSERT_THAT(params.findParameterIndexByName("A"), Eq(0));
    ASSERT_THAT(params.findParameterIndexByName("B"), Eq(1));
}

TEST_F(AParameters, ReturnsNeg1IfCannotFindByName) {
    params.pushFloatParameter("A", 1.0, 0, 2.0);
    params.pushBoolParameter("B", true);

    ASSERT_THAT(params.findParameterIndexByName("C"), Eq(-1));
}

TEST_F(AParameters, CanAddIntParameter) {
    params.pushIntParameter("int", 0, -1, 10);

    AudioProcessorParameter &p = params.get(0);
    AudioProcessorParameter expected{
        ParameterType::kInt, 0, "int", 0, -1, 10.0f};

    ASSERT_THAT(p, Eq(expected));
}

TEST_F(AParameters, CanGetParameterByName) {
    params.pushFloatParameter("A", 1.0, 0, 2.0);
    params.pushBoolParameter("B", true);

    ASSERT_THAT(params.get("A").getParameterName(), Eq("A"));
    ASSERT_THAT(params.get("B").getParameterName(), Eq("B"));
}

TEST_F(AParameters, GetByNameThrowsIfCannotFindParameter) {
    params.pushFloatParameter("A", 1.0, 0, 2.0);
    params.pushBoolParameter("B", true);

    ASSERT_ANY_THROW(params.get("C"));
}