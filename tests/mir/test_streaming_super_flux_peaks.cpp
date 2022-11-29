//
// Created by user on 6/29/22.
//
#include "libaa/mir/aa_streaming_superflux_peaks.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;
class AStreamingSuperFluxPeaks : public Test {
public:
    StreamingSuperFluxPeaks s;
};

TEST_F(AStreamingSuperFluxPeaks, CanReportCorrectName) {
    ASSERT_THAT(s.getName(), Eq("StreamingSuperFluxPeaks"));
}

TEST_F(AStreamingSuperFluxPeaks, InputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(s.input("novelty").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingSuperFluxPeaks, OutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(s.output("peaks").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingSuperFluxPeaks, CombineParameterAsExpected) {
    auto &p = s.getParameter("combine");

    ASSERT_THAT(p.getParameterName(), Eq("combine"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), Eq(30));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, FrameRateParameterAsExpected) {
    auto &p = s.getParameter("frameRate");

    ASSERT_THAT(p.getParameterName(), Eq("frameRate"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(172.265));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, PreAverageParameterAsExpected) {
    auto &p = s.getParameter("preAverage");

    ASSERT_THAT(p.getParameterName(), Eq("preAverage"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(100));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, PreMaxParameterAsExpected) {
    auto &p = s.getParameter("preMax");

    ASSERT_THAT(p.getParameterName(), Eq("preMax"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(30));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, RatioThresholdAsExpected) {
    auto &p = s.getParameter("ratioThreshold");

    ASSERT_THAT(p.getParameterName(), Eq("ratioThreshold"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatEq(16));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, ThresholdParameterAsExpected) {
    auto &p = s.getParameter("threshold");

    ASSERT_THAT(p.getParameterName(), Eq("threshold"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatNear(0.05, 1e-6));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(AStreamingSuperFluxPeaks, ConfigureInternalMovingAverage) {
    s.configure();

    float frame_rate = s.getParameter("frameRate").getPlainValue();
    float pre_ave_ms = s.getParameter("preAverage").getPlainValue();
    int pre_ave_size = int(frame_rate * pre_ave_ms / 1000.0f);
    ASSERT_THAT(s.getStreamingMovingAverage()->getParameter("size").getInt(), Eq(pre_ave_size));
}

TEST_F(AStreamingSuperFluxPeaks, ConfigureInternalMaxFilter) {
    s.configure();

    float frame_rate = s.getParameter("frameRate").getPlainValue();
    float pre_max_ms = s.getParameter("preMax").getPlainValue();
    int pre_max_size = int(frame_rate * pre_max_ms / 1000.0f);
    ASSERT_THAT(s.getStreamingMaxFilter()->getParameter("width").getInt(), Eq(pre_max_size));
}

TEST_F(AStreamingSuperFluxPeaks, ComputeGetPeaks) {
    std::vector<float> test_novelty{-0.999984,
                                    -0.736924,
                                    0.511211,
                                    -0.0826997,
                                    0.0655345,
                                    -0.562082,
                                    -0.905911,
                                    0.357729,
                                    0.358593,
                                    0.869386};
    float in;
    std::vector<float> peaks;
    std::vector<float> all_peaks;
    s.input("novelty").set(in);
    s.output("peaks").set(peaks);
    s.configure();

    for (int i = 0; i < test_novelty.size(); ++i) {
        in = test_novelty[i];
        s.compute();

        if (!peaks.empty()) {
            all_peaks.insert(all_peaks.end(), peaks.begin(), peaks.end());
        }
    }

    ASSERT_THAT(all_peaks.size(), Eq(2));
    ASSERT_THAT(all_peaks[0], FloatNear(0.01161, 1e-6));
    ASSERT_THAT(all_peaks[1], FloatNear(0.0464401, 1e-6));
}

TEST_F(AStreamingSuperFluxPeaks, ResetResetInternalPeakTime) {
    std::vector<float> test_novelty{-0.999984,
                                    -0.736924,
                                    0.511211,
                                    -0.0826997,
                                    0.0655345,
                                    -0.562082,
                                    -0.905911,
                                    0.357729,
                                    0.358593,
                                    0.869386};
    float in;
    std::vector<float> peaks;
    std::vector<float> all_peaks;
    s.input("novelty").set(in);
    s.output("peaks").set(peaks);
    s.configure();

    for (int i = 0; i < test_novelty.size(); ++i) {
        in = test_novelty[i];
        s.compute();
    }
    ASSERT_THAT(s.getStartPeakTime(), Not(0.0f));

    s.reset();

    ASSERT_THAT(s.getStartPeakTime(), Eq(0.0f));
}