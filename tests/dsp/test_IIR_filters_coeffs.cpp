
//
// Created by William.Hua on 2020/12/20.
//

#include "libaa/audio_effect/aa_iir_filter.h"
#include "../aa_test_helper.h"
#include <array>
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AIIRFilter : public Test
{
public:
    IIRFilter filter;
    IIRFilterParameter iir_params;
};

TEST_F(AIIRFilter, CanCalc1stOrderLPFCoefficents)
{
    iir_params = {FilterType::kLPF1,
                  1000.0f,
                  1.0f,
                  10.0f};
    float sample_rate = 2000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{1, 1, 0.0, 1.0, 0.0, 1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc1stOrderHPFCoefficents)
{
    iir_params = {FilterType::kHPF1,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 2000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0, 0, 0.0, 1.0, 0.0, 1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderLPFCoefficents)
{
    iir_params = {FilterType::kLPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 2000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{1.0, 2, 1, 2, 1,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderHPFCoefficents)
{
    iir_params = {FilterType::kHPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 2000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0, 0, 0, 2, 1,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderBPFCoefficents)
{
    iir_params = {FilterType::kBPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{1.0f/3, 0, -1.0f/3, 0, 1.0f/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderBSFCoefficents)
{
    iir_params = {FilterType::kBSF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{2.0f/3, 0, 2.0f/3, 0, 1.0f/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthLPFCoefficents)
{
    iir_params = {FilterType::kButterLPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.2928, 0.5857, 0.2928, 0, 0.1715,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthHPFCoefficents)
{
    iir_params = {FilterType::kButterHPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.2928, -0.5857, 0.2928, 0, 0.1715,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthBPFCoefficents)
{
    iir_params = {FilterType::kButterBPF2,
                  20.0f,
                  2.0f,
                  10.0f};

    float sample_rate = 800.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.5, 0.0, -0.5, -0.9876883405951378, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthBSFCoefficents)
{
    iir_params = {FilterType::kButterBSF2,
                  20.0f,
                  2.0f,
                  10.0f};

    float sample_rate = 800.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.5, -0.9876883405951378, 0.5, -0.9876883405951378, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcLWRLPF2Coefficents)
{
    iir_params = {FilterType::kLWRLPF2,
                  1000.0f,
                  2.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.25, 0.5, 0.25, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalckLWRHPF2Coefficents)
{
    iir_params = {FilterType::kLWRLPF2,
                  1000.0f,
                  2.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.25, 0.5, 0.25, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc1srOrderAPFCoefficents)
{
    iir_params = {FilterType::kAPF1,
                  1000.0f,
                  2.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.0, 1.0, 0.0, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderkAPFCoefficents)
{
    iir_params = {FilterType::kAPF2,
                  1000.0f,
                  1.0f,
                  10.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.0, 0.0, 1.0, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcLowShelfvingCoefficents)
{
    iir_params = {FilterType::kLowShelf,
                  1000.0f,
                  1.0f,
                  0.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{2.0/3, 2.0/3, 0.0, 1.0/3, 0.0,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcHighShelfvingCoefficents)
{
    iir_params = {FilterType::kHighShelf,
                  1000.0f,
                  1.0f,
                  0.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{1.0/3, -1.0/3, 0.0, 1.0/3, 0.0,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderNonConstantQCoefficents)
{
    iir_params = {FilterType::kNCQParaEQ,
                  1000.0f,
                  1.0f,
                  0.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{2.0/3, 0.0, -2.0/3, 0.0, -1.0/3,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderConstantQWithBoostCoefficents)
{
    iir_params = {FilterType::kCQParaEQ,
                  1000.0f,
                  1.0f,
                  20.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{4, 0.0, -8.0/3, 0.0, 1.0/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderConstantQWithCutCoefficents)
{
    iir_params = {FilterType::kCQParaEQ,
                  1000.0f,
                  1.0f,
                  -20.0f};

    float sample_rate = 4000.0f;
    auto result = filter.calcFilterCoeffs(iir_params, sample_rate);

    auto expected_result = FilterCoeffs{0.25, 0.0, 1.0/12, 0.0, -2.0/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}