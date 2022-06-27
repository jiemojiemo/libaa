//
// Created by user on 6/25/22.
//

#include "libaa/mir/aa_superflux_novelty.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class ASuperFluxNovelty : public Test {
public:
    SuperFluxNovelty s;
};

TEST_F(ASuperFluxNovelty, CanReportCorrectName) {
    ASSERT_THAT(s.getName(), Eq("SuperFluxNovelty"));
}

TEST_F(ASuperFluxNovelty, BandsInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<std::vector<float>>);

    ASSERT_THAT(s.input("bands").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASuperFluxNovelty, DifferencesInputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(s.output("differences").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASuperFluxNovelty, BindWidthParameterAsExpected) {
    auto &p = s.getParameter("binWidth");

    ASSERT_THAT(p.getParameterName(), Eq("binWidth"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(3));
    ASSERT_THAT(p.getMinPlainValue(), Eq(3));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ASuperFluxNovelty, FrameWidthParameterAsExpected) {
    auto &p = s.getParameter("frameWidth");

    ASSERT_THAT(p.getParameterName(), Eq("frameWidth"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(2));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ASuperFluxNovelty, CofigureInternalMaxFilter) {
    int width = 10;
    s.setParameter("binWidth", width);
    s.configure();

    ASSERT_THAT(s.getMaxFilter()->getParameter("width").getInt(), Eq(width));
}

TEST_F(ASuperFluxNovelty, ComputeGetDiff) {
    s.configure();
    std::vector<std::vector<float>> bands{
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2},
    };
    float out{0.0f};
    s.input("bands").set(bands);
    s.output("differences").set(out);

    s.compute();

    ASSERT_THAT(out, Eq(10));
}

TEST_F(ASuperFluxNovelty, ComputeThrowsIfNotEnoughFrameToCompute) {
    s.configure();
    std::vector<std::vector<float>> bands{
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
    };
    float out{0.0f};
    s.input("bands").set(bands);
    s.output("differences").set(out);

    ASSERT_THAT(s.getParameter("frameWidth").getInt(), Ge(bands.size()));
    ASSERT_ANY_THROW(s.compute());
}