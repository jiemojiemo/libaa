//
// Created by user on 6/24/22.
//
#include "libaa/mir/aa_spectrum.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class ASpectrum : public Test {
public:
    Spectrum spec;
};

TEST_F(ASpectrum, CanReportCorrectName) {
    ASSERT_THAT(spec.getName(), Eq("Spectrum"));
}

TEST_F(ASpectrum, FrameInputAsExpected) {
    const auto &expected = typeid(std::vector<float>);

    ASSERT_THAT(spec.input("frame").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASpectrum, SpectrumOutputAsExpected) {
    const auto &expected = typeid(std::vector<float>);

    ASSERT_THAT(spec.output("spectrum").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASpectrum, SizeParameterAsExpected) {
    auto &p = spec.getParameter("size");

    ASSERT_THAT(p.getParameterName(), Eq("size"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(2048));
    ASSERT_THAT(p.getMinPlainValue(), Eq(1));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ASpectrum, ConfigureInternalFFT) {
    spec.configure();

    auto expected = spec.getParameter("size").getInt();
    ASSERT_THAT(spec.getFFT()->getParameter("size").getInt(), Eq(expected));
}

TEST_F(ASpectrum, ConfigureSetFFTOutput) {
    spec.configure();

    ASSERT_THAT(spec.getFFT()->output("fft").getRawData(), NotNull());
}

TEST_F(ASpectrum, ConfigureSetMagnitudeInput) {
    spec.configure();

    ASSERT_THAT(spec.getMagnitude()->input("complex").getRawData(), NotNull());
}

TEST_F(ASpectrum, ComputeGetSpectrum) {
    std::vector<float> frame{1, 0, 0, 0};
    std::vector<float> spectrum;
    spec.setParameter("size", frame.size());
    spec.input("frame").set(frame);
    spec.output("spectrum").set(spectrum);
    spec.configure();

    spec.compute();

    std::vector<float> expected{1, 1, 1};
    ASSERT_THAT(spectrum.size(), Eq(frame.size() / 2 + 1));
    ASSERT_THAT(spectrum, ContainerEq(expected));
}