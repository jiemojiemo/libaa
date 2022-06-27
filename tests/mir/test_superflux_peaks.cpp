//
// Created by user on 6/25/22.
//
#include "libaa/mir/aa_superflux_peaks.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace std;
using namespace libaa::MIR;

class ASuperFluxPeaks : public Test {
public:
    SuperFluxPeaks s;
};

TEST_F(ASuperFluxPeaks, CanReportCorrectName) {
    ASSERT_THAT(s.getName(), Eq("SuperFluxPeaks"));
}

TEST_F(ASuperFluxPeaks, InputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(s.input("novelty").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASuperFluxPeaks, OutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(s.output("peaks").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(ASuperFluxPeaks, CombineParameterAsExpected) {
    auto &p = s.getParameter("combine");

    ASSERT_THAT(p.getParameterName(), Eq("combine"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), Eq(30));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, FrameRateParameterAsExpected) {
    auto &p = s.getParameter("frameRate");

    ASSERT_THAT(p.getParameterName(), Eq("frameRate"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(172.265));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, PreAverageParameterAsExpected) {
    auto &p = s.getParameter("preAverage");

    ASSERT_THAT(p.getParameterName(), Eq("preAverage"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(100));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, PreMaxParameterAsExpected) {
    auto &p = s.getParameter("preMax");

    ASSERT_THAT(p.getParameterName(), Eq("preMax"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(30));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, RatioThresholdAsExpected) {
    auto &p = s.getParameter("ratioThreshold");

    ASSERT_THAT(p.getParameterName(), Eq("ratioThreshold"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(16));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, ThresholdParameterAsExpected) {
    auto &p = s.getParameter("threshold");

    ASSERT_THAT(p.getParameterName(), Eq("threshold"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatNear(0.05, 1e-6));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxPeaks, ConfigureInternalMovingAverage) {
    s.configure();

    float frame_rate = s.getParameter("frameRate").getPlainValue();
    float pre_ave_ms = s.getParameter("preAverage").getPlainValue();
    int pre_ave_size = int(frame_rate * pre_ave_ms / 1000.0f);
    ASSERT_THAT(s.getMovingAverage()->getParameter("size").getInt(), Eq(pre_ave_size));
}

TEST_F(ASuperFluxPeaks, ConfigureInternalMaxFilter) {
    s.configure();

    float frame_rate = s.getParameter("frameRate").getPlainValue();
    float pre_max_ms = s.getParameter("preMax").getPlainValue();
    int pre_max_size = int(frame_rate * pre_max_ms / 1000.0f);
    ASSERT_THAT(s.getMaxFilter()->getParameter("width").getInt(), Eq(pre_max_size));
}

TEST_F(ASuperFluxPeaks, ComputeGetPeaks) {
    std::vector<float> novelty{-0.999984,
                               -0.736924,
                               0.511211,
                               -0.0826997,
                               0.0655345,
                               -0.562082,
                               -0.905911,
                               0.357729,
                               0.358593,
                               0.869386};
    std::vector<float> peaks;

    s.input("novelty").set(novelty);
    s.output("peaks").set(peaks);
    s.configure();
    s.compute();

    ASSERT_THAT(peaks.size(), Eq(2));
    ASSERT_THAT(peaks[0], FloatNear(0.01161, 1e-6));
    ASSERT_THAT(peaks[1], FloatNear(0.0464401, 1e-6));
}