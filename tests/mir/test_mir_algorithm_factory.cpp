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
    auto name = "FFT";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateManitude) {
    auto name = "Magnitude";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateMaxFilter) {
    auto name = "MaxFilter";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateMovingAverage) {
    auto name = "MovingAverage";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateSpectrum) {
    auto name = "Spectrum";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateStreamingMovingAverage) {
    auto name = "StreamingMaxFilter";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateStreamingMaxFilter) {
    auto name = "StreamingMaxFilter";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
    ASSERT_THAT(MIRAlgorithmFactory::create("StreamingMovingAverage"), NotNull());
}

TEST_F(AMIRAlgorithmFactory, CanCreateWindowing) {
    auto name = "Windowing";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateTriangularBands) {
    auto name = "TriangularBands";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateSuperFluxNovelty) {
    auto name = "SuperFluxNovelty";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateStreamingSuperFluxNovelty) {
    auto name = "StreamingSuperFluxNovelty";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateSuperFluxPeaks) {
    auto name = "SuperFluxPeaks";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}

TEST_F(AMIRAlgorithmFactory, CanCreateStreamingSuperFluxPeaks) {
    auto name = "StreamingSuperFluxPeaks";
    ASSERT_THAT(MIRAlgorithmFactory::create(name)->getName(), Eq(name));
}
