//
// Created by user on 4/18/22.
//
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

    ASSERT_THAT(f.processSample(1.0f), FloatEq(0.00707352));
    ASSERT_THAT(f.processSample(1.0f), FloatEq(0.0211205));
}