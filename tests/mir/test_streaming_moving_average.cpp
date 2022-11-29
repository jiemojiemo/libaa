//
// Created by user on 6/29/22.
//
#include "../aa_testing_helper.h"
#include "libaa/mir/aa_streaming_moving_average.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
using namespace libaa::MIR;
class AStreamingMovingAverage : public Test {
public:
    StreamingMovingAverage m;
};

TEST_F(AStreamingMovingAverage, CanReportCorrectName) {
    ASSERT_THAT(m.getName(), Eq("StreamingMovingAverage"));
}

TEST_F(AStreamingMovingAverage, SignalInputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(m.input("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingMovingAverage, SignalOutputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(m.output("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingMovingAverage, SizeParameterAsExpected) {
    auto &p = m.getParameter("size");

    ASSERT_THAT(p.getParameterName(), Eq("size"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(6));
    ASSERT_THAT(p.getMinPlainValue(), Eq(1));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AStreamingMovingAverage, ConfigureInternlDelayLine) {
    int size = 8;
    m.setParameter("size", size);

    m.configure();

    ASSERT_THAT(m.getDelayLine().size(), Eq(size));
}

TEST_F(AStreamingMovingAverage, ComputeAsExpected) {
    float in;
    float out;
    m.configure();
    m.input("signal").set(in);
    m.output("signal").set(out);

    std::vector<float> in_signal{0.1, 0.2, 0.3, 0.3, 0.3, 0.01, 0.01, 0.02};
    std::vector<float> expected{0.0166667, 0.05, 0.1, 0.15, 0.2, 0.201667, 0.186667, 0.156667};

    for (int i = 0; i < in_signal.size(); ++i) {
        in = in_signal[i];
        m.compute();

        ASSERT_THAT(out, FloatNear(expected[i], 1e-6));
    }
}

TEST_F(AStreamingMovingAverage, ResetClearInternalMean) {
    m.configure();
    float in;
    float out;
    m.input("signal").set(in);
    m.output("signal").set(out);

    std::vector<float> test_signal(10, 1.0f);

    for (int i = 0; i < test_signal.size(); ++i) {
        in = test_signal[i];
        m.compute();
    }

    m.reset();

    ASSERT_THAT(m.getPreMean(), Eq(0.0f));
}