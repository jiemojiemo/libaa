//
// Created by user on 4/16/22.
//
#include "libaa/dsp/aa_simple_LPF.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ASimpleLPF : public Test {
public:
    SimpleLPF f;
};

TEST_F(ASimpleLPF, CanUpdateParameters) {
    SimpleLPF::SimpleLPFParameters parameters{};
    parameters.g = 1.0f;

    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().g, Eq(parameters.g));
}

TEST_F(ASimpleLPF, ProcessGetFilteredSample) {
    SimpleLPF::SimpleLPFParameters parameters{};
    parameters.g = 0.5f;
    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0f), Eq(0.5));
    ASSERT_THAT(f.processSample(1.0f), Eq(0.75));
}