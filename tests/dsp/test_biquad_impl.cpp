//
// Created by user on 4/20/22.
//
#include "libaa/dsp/aa_biquad_impl.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class ABiquad : public Test {
public:
    Biquad b;
    FilterCoeffs coeffs{0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7};
};

TEST_F(ABiquad, InitCoefficientsAreAllZero) {
    std::array<float, num_coeffs> expected{{
        0.0f,
    }};

    ASSERT_THAT(b.coeff_array, ContainerEq(expected));
}

TEST_F(ABiquad, InitStateAreAllZero) {
    std::array<float, num_states> expected{{
        0.0f,
    }};

    ASSERT_THAT(b.state_array, ContainerEq(expected));
}

TEST_F(ABiquad, CanUpdateCoefficients) {
    b.updateCoeffs(coeffs);

    ASSERT_THAT(b.coeff_array, ContainerEq(coeffs));
}

TEST_F(ABiquad, ResetMakeStatesAndCoeffsToZero) {
    b.coeff_array[0] = 1;
    b.state_array[0] = 1;

    b.reset();

    ASSERT_THAT(b.coeff_array[0], Eq(0));
    ASSERT_THAT(b.state_array[0], Eq(0));
}

TEST_F(ABiquad, ProcessSampleWithDirectForm) {
    b.updateCoeffs(coeffs);

    ASSERT_THAT(b.processSampleWithDirectForm(1.0f), Eq(0.1f));
}

TEST_F(ABiquad, DefaultProcessUseDirectForm) {
    Biquad b0;

    b.updateCoeffs(coeffs);
    b0.updateCoeffs(coeffs);

    ASSERT_THAT(b.processSample(1.0), Eq(b0.processSampleWithDirectForm(1.0)));
}

TEST_F(ABiquad, ProcessSampleWithCanonicalForm) {
    Biquad b0;

    b.updateCoeffs(coeffs);
    b0.updateCoeffs(coeffs);

    ASSERT_THAT(b.processSample(1.0),
                Eq(b0.processSampleWithCanonicalForm(1.0)));
}

TEST_F(ABiquad, ProcessSampleWithTransposedCanonicalForm) {
    Biquad b0;

    b.updateCoeffs(coeffs);
    b0.updateCoeffs(coeffs);

    ASSERT_THAT(b.processSample(1.0),
                Eq(b0.processSampleWithTransposedCanonicalForm(1.0)));
}

TEST_F(ABiquad, ProcessSampleWithTransposedDirectForm) {
    Biquad b0;

    b.updateCoeffs(coeffs);
    b0.updateCoeffs(coeffs);

    ASSERT_THAT(b.processSample(1.0),
                Eq(b0.processSampleWithTransposedDirectForm(1.0)));
}