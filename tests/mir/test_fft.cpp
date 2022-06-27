//
// Created by user on 6/22/22.
//
#include "../aa_testing_helper.h"
#include "libaa/mir/aa_fft.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;
class AFFT : public Test {
public:
    FFT fft;
};

TEST_F(AFFT, CanReportCorrectName) {
    ASSERT_THAT(fft.getName(), Eq("FFT"));
}

TEST_F(AFFT, FrameInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(fft.input("frame").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AFFT, FFTOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<std::complex<float>>);

    ASSERT_THAT(fft.output("fft").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AFFT, SizeParameterAsExpected) {
    auto &p = fft.getParameter("size");

    ASSERT_THAT(p.getParameterName(), Eq("size"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(1024));
    ASSERT_THAT(p.getMinPlainValue(), Eq(1));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AFFT, ConfigureCreateInternalFFTImpl) {
    ASSERT_THAT(fft.getFFTImpl(), IsNull());

    fft.configure();

    ASSERT_THAT(fft.getFFTImpl(), NotNull());
}

TEST_F(AFFT, ComputeWillResizeOutFFT) {
    std::vector<float> signal{0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<std::complex<float>> out_fft;
    fft.setParameter("size", signal.size());
    fft.configure();

    fft.input("frame").set(signal);
    fft.output("fft").set(out_fft);

    fft.compute();

    ASSERT_THAT(out_fft.size(), Eq(signal.size() / 2 + 1));
}

TEST_F(AFFT, ComputeThrowsIfSignalSizeLessThanFFTSize) {
    std::vector<float> signal{0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<std::complex<float>> out_fft;
    fft.setParameter("size", 128);
    fft.configure();

    fft.input("frame").set(signal);
    fft.output("fft").set(out_fft);

    ASSERT_ANY_THROW(fft.compute());
}

TEST_F(AFFT, ComputeFFTOut) {
    std::vector<float> signal{0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<std::complex<float>> out_fft;
    fft.setParameter("size", signal.size());
    fft.configure();

    fft.input("frame").set(signal);
    fft.output("fft").set(out_fft);

    fft.compute();

    std::vector<std::complex<float>> expected{
        {28, 0}, {-4, 9.65685}, {-4, 4}, {-4, 1.65685}, {-4, 0}};

    ASSERT_THAT(out_fft, Pointwise(NearWithPrecision(1e-5), expected));
}
