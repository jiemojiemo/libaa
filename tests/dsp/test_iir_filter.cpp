//
// Created by user on 4/18/22.
//
#include "aa_testing_helper.h"
#include "libaa/dsp/aa_iir_filter.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AIIRFilter : public Test {
public:
    IIRFilter f;
    IIRFilter::IIRFilterParameters parameters;
    const float sample_rate = 44100;
};

TEST_F(AIIRFilter, NeedPrepareBeforeUse) {
    f.prepare(sample_rate);
}

TEST_F(AIIRFilter, CanUpdateParameters) {
    IIRFilter::IIRFilterParameters parameters;
    parameters.type = FilterType::kLPF1;
    parameters.Q = 1.0f;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().type, Eq(parameters.type));
    ASSERT_THAT(f.getParameters().Q, Eq(parameters.Q));
}

TEST_F(AIIRFilter, UpdateParameterAlsoUpdateFilterCoeffcient) {
    parameters.type = FilterType::kLPF1;
    parameters.Q = 1.0f;

    f.prepare(sample_rate);
    f.updateParameters(parameters);

    auto expected_coeffs =
        IIRFilter::calcFilterCoefficients(parameters, sample_rate);
    ASSERT_THAT(f.getBiquad().coeff_array, Eq(expected_coeffs));
}

TEST_F(AIIRFilter, ProcessGetFilteredSample) {
    parameters.type = FilterType::kLPF1;
    parameters.fc = 100;
    f.prepare(sample_rate);
    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0f), FloatEq(0.00707352f));
}

TEST_F(AIIRFilter, CanGetkLPF1Coeffs) {
    parameters.type = FilterType::kLPF1;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.00707352, 0.00707352, 0, -0.985853,
                                     0, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-6), expected));
}

TEST_F(AIIRFilter, CanGetkHPF1Coeffs) {
    parameters.type = FilterType::kHPF1;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.992926, -0.992926, 0, -0.985853,
                                     0, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-6), expected));
}

TEST_F(AIIRFilter, CanGetkLPF2Coeffs) {
    parameters.type = FilterType::kLPF2;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {
        5.02413e-05, 0.000100483, 5.02413e-05, -1.97985, 0.98005, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkHPF2Coeffs) {
    parameters.type = FilterType::kHPF2;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.989974, -1.97995, 0.989974, -1.97985,
                                     0.98005, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkBPF2Coeffs) {
    parameters.type = FilterType::kBPF2;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {
        0.00997524, 0, -0.00997524, -1.97985, 0.98005, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkBSF2Coeffs) {
    parameters.type = FilterType::kBSF2;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.990025, -1.97985, 0.990025, -1.97985,
                                     0.98005, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkAPF1Coeffs) {
    parameters.type = FilterType::kAPF1;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {-0.985853, 1, 0, -0.985853, 0, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkAPF2Coeffs) {
    parameters.type = FilterType::kAPF2;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.980048, -1.97985, 1, -1.97985,
                                     0.980048, 1, 0};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkLowShelfCoeffs) {
    parameters.type = FilterType::kLowShelf;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.0140477, 0.0140477, 0, -0.971905,
                                     0, 0, 1};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AIIRFilter, CanGetkHighShelfCoeffs) {
    parameters.type = FilterType::kHighShelf;

    auto coeffs = f.calcFilterCoefficients(parameters, sample_rate);
    std::array<float, 7> expected = {0.996451, -0.996451, 0, -0.992901,
                                     0, 0, 1};
    ASSERT_THAT(coeffs, Pointwise(NearWithPrecision(1e-5), expected));
}