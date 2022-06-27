//
// Created by user on 6/23/22.
//
#include "libaa/mir/aa_magnitude.h"
#include <gmock/gmock.h>

using namespace testing;

using namespace libaa::MIR;

class AMagnitude : public Test {
public:
    Magnitude mag;
};

TEST_F(AMagnitude, CanReportCorrectName) {
    ASSERT_THAT(mag.getName(), Eq("Magnitude"));
}

TEST_F(AMagnitude, ComplexInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<std::complex<float>>);

    ASSERT_THAT(mag.input("complex").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMagnitude, MagnitudeOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(mag.output("magnitude").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMagnitude, ComputeResizeOutputMagSize) {
    int size = 16;
    std::vector<std::complex<float>> complex(size, {0, 0});
    std::vector<float> out_mag;
    mag.input("complex").set(complex);
    mag.output("magnitude").set(out_mag);
    mag.configure();

    mag.compute();

    ASSERT_THAT(out_mag.size(), Eq(size));
}

TEST_F(AMagnitude, ComputeGetMagnitudeOutput) {
    std::vector<std::complex<float>> complex{{1, 1}, {2, 2}};
    std::vector<float> out_mag;
    mag.input("complex").set(complex);
    mag.output("magnitude").set(out_mag);
    mag.configure();

    mag.compute();

    ASSERT_THAT(out_mag[0], Eq(abs(complex[0])));
    ASSERT_THAT(out_mag[1], Eq(abs(complex[1])));
}