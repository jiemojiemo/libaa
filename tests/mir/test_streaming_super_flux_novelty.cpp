//
// Created by user on 6/29/22.
//
#include "libaa/mir/aa_streaming_superflux_novelty.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
using namespace libaa::MIR;

class AStreamingSuperFluxNovelty : public Test {
public:
    StreamingSuperFluxNovelty s;
};

TEST_F(AStreamingSuperFluxNovelty, CanReportCorrectName) {
    ASSERT_THAT(s.getName(), Eq("StreamingSuperFluxNovelty"));
}

TEST_F(AStreamingSuperFluxNovelty, BandsInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(s.input("bands").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingSuperFluxNovelty, DifferencesInputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(s.output("differences").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingSuperFluxNovelty, BindWidthParameterAsExpected) {
    auto &p = s.getParameter("binWidth");

    ASSERT_THAT(p.getParameterName(), Eq("binWidth"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(3));
    ASSERT_THAT(p.getMinPlainValue(), Eq(3));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AStreamingSuperFluxNovelty, FrameWidthParameterAsExpected) {
    auto &p = s.getParameter("frameWidth");

    ASSERT_THAT(p.getParameterName(), Eq("frameWidth"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(2));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AStreamingSuperFluxNovelty, ConfigureInternalSuperFluxNovelty) {
    int frame_width = 5;
    int width = 8;
    s.setParameter("frameWidth", frame_width);
    s.setParameter("binWidth", width);
    s.configure();

    ASSERT_THAT(s.getSuperFluxNovelty()->getParameter("frameWidth").getInt(), Eq(frame_width));
    ASSERT_THAT(s.getSuperFluxNovelty()->getParameter("binWidth").getInt(), Eq(width));
}

TEST_F(AStreamingSuperFluxNovelty, ComputeGetDiff) {
    s.configure();
    std::vector<std::vector<float>> bands{
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2},
    };
    std::vector<float> expected_out{0.0f, 0.0f, 10.0f};

    for (int i = 0; i < bands.size(); ++i) {
        float out{0.0f};
        s.input("bands").set(bands[i]);
        s.output("differences").set(out);
        s.compute();

        ASSERT_THAT(out, Eq(expected_out[i]));
    }
}

TEST_F(AStreamingSuperFluxNovelty, ResetClearRingBands) {
    s.configure();
    std::vector<std::vector<float>> bands{
        {0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {2, 2, 2, 2, 2},
    };
    std::vector<float> expected_out{0.0f, 0.0f, 10.0f};
    for (int i = 0; i < bands.size(); ++i) {
        float out{0.0f};
        s.input("bands").set(bands[i]);
        s.output("differences").set(out);
        s.compute();
    }

    s.reset();

    ASSERT_THAT(s.getRingBands().size(), Eq(0));
}