//
// Created by user on 6/24/22.
//

#include "../aa_testing_helper.h"
#include "libaa/mir/aa_triangular_bands.h"
#include <Eigen/Dense>
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class ATriangularBands : public Test {
public:
    void SetUp() override {
        in_spec.resize(1025, 0.1f);
    }
    TriangularBands tri;
    std::vector<float> in_spec;
    std::vector<float> out_bands;
};

TEST_F(ATriangularBands, CanReportCorrectName) {
    ASSERT_THAT(tri.getName(), Eq("TriangularBands"));
}

TEST_F(ATriangularBands, SpectrumInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(tri.input("spectrum").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ATriangularBands, BandsOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(tri.output("bands").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ATriangularBands, InputSizeParameterAsExpected) {
    auto &p = tri.getParameter("inputSize");

    ASSERT_THAT(p.getParameterName(), Eq("inputSize"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(1025));
    ASSERT_THAT(p.getMinPlainValue(), Eq(1));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ATriangularBands, LogParameterAsExpected) {
    auto &p = tri.getParameter("log");

    ASSERT_THAT(p.getParameterName(), Eq("log"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kBool));
    ASSERT_THAT(p.getBool(), Eq(true));
}

TEST_F(ATriangularBands, SampleRateParameterAsExpected) {
    auto &p = tri.getParameter("sampleRate");

    ASSERT_THAT(p.getParameterName(), Eq("sampleRate"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), Eq(44100.0f));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ATriangularBands, TypeParameterAsExpected) {
    auto &p = tri.getParameter("type");

    ASSERT_THAT(p.getParameterName(), Eq("type"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kChoice));
    ASSERT_THAT(p.getChoiceString(), Eq("power"));
    ASSERT_THAT(p.getChoiceStrings(), ElementsAre("magnitude", "power"));
}

TEST_F(ATriangularBands, ConfigureCalcNumberBands) {
    const auto &frequencies = tri.getFrequencies();

    tri.configure();

    int expected_num_bands = frequencies.size() - 2;
    ASSERT_THAT(tri.getNumberBands(), Eq(expected_num_bands));
}

TEST_F(ATriangularBands, ConfigureCreateInternalFilters) {
    tri.configure();

    const Eigen::MatrixXf &filters = *static_cast<const Eigen::MatrixXf *>(tri.getFilters());

    ASSERT_THAT(filters.rows(), Eq(tri.getNumberBands()));
    ASSERT_THAT(filters.cols(), Eq(tri.getParameter("inputSize").getInt()));

    // same as essentia TriangularBands
    ASSERT_THAT(filters(0, 2), Eq(1.0f));
    ASSERT_THAT(filters(1, 3), Eq(1.0f));
}

TEST_F(ATriangularBands, CanCreateFiltersInLowSampleRate) {
    tri.setParameter("sampleRate", 16000);
    tri.configure();

    auto *filters = static_cast<const Eigen::MatrixXf *>(tri.getFilters());
    ASSERT_THAT(filters->rows(), Eq(114));
}

TEST_F(ATriangularBands, ComputeGetTriangularBands) {
    tri.setParameter("inputSize", in_spec.size());
    tri.configure();
    tri.input("spectrum").set(in_spec);
    tri.output("bands").set(out_bands);

    tri.compute();

    std::vector<float> expected(tri.getNumberBands(), 0.0143553);
    ASSERT_THAT(out_bands.size(), Eq(tri.getNumberBands()));
    ASSERT_THAT(out_bands, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(ATriangularBands, ComputeWithoutLog) {
    tri.setParameter("inputSize", in_spec.size());
    tri.setParameter("log", false);
    tri.configure();
    tri.input("spectrum").set(in_spec);
    tri.output("bands").set(out_bands);

    tri.compute();

    std::vector<float> expected(tri.getNumberBands(), 0.01);
    ASSERT_THAT(out_bands, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(ATriangularBands, ComputeWithMagnitude) {
    tri.setParameter("inputSize", in_spec.size());
    tri.setParameter("type", "magnitude");
    tri.configure();
    tri.input("spectrum").set(in_spec);
    tri.output("bands").set(out_bands);

    tri.compute();

    std::vector<float> expected(tri.getNumberBands(), 0.137504);
    ASSERT_THAT(out_bands, Pointwise(NearWithPrecision(1e-5), expected));
}