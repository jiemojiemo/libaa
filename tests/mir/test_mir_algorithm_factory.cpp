//
// Created by user on 6/24/22.
//
#include "libaa/mir/aa_mir_algorithm.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;
class AMIRAlgorithmFactory : public Test {
public:
};

TEST_F(AMIRAlgorithmFactory, ThrowsIfNotSuchAlgorithm) {
    ASSERT_ANY_THROW(MIRAlgorithmFactory::create("XXXYYY"));
}

TEST_F(AMIRAlgorithmFactory, CanCreateFFT) {
    ASSERT_THAT(MIRAlgorithmFactory::create("FFT"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateManitude) {
    ASSERT_THAT(MIRAlgorithmFactory::create("Magnitude"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateMaxFilter) {
    ASSERT_THAT(MIRAlgorithmFactory::create("MaxFilter"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateMovingAverage) {
    ASSERT_THAT(MIRAlgorithmFactory::create("MovingAverage"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateSpectrum) {
    ASSERT_THAT(MIRAlgorithmFactory::create("Spectrum"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateWindowing) {
    ASSERT_THAT(MIRAlgorithmFactory::create("Windowing"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateTriangularBands) {
    ASSERT_THAT(MIRAlgorithmFactory::create("TriangularBands"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateSuperFluxNovelty) {
    ASSERT_THAT(MIRAlgorithmFactory::create("SuperFluxNovelty"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateSuperFluxPeaks) {
    ASSERT_THAT(MIRAlgorithmFactory::create("SuperFluxPeaks"), NotNull());
}
