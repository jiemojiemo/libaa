//
// Created by user on 6/25/22.
//
#include "../aa_testing_helper.h"
#include "libaa/mir/aa_moving_average.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;
using namespace libaa;
class AMovingAverage : public Test {
public:
    MovingAverage m;
};

TEST_F(AMovingAverage, CanReportCorrectName) {
    ASSERT_THAT(m.getName(), Eq("MovingAverage"));
}

TEST_F(AMovingAverage, SignalInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(m.input("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMovingAverage, SignalOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(m.output("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMovingAverage, SizeParameterAsExpected) {
    auto &p = m.getParameter("size");

    ASSERT_THAT(p.getParameterName(), Eq("size"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(6));
    ASSERT_THAT(p.getMinPlainValue(), Eq(1));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AMovingAverage, ComputeAsExpected) {
    m.configure();
    std::vector<float> in_signal(10, 1.0f);
    std::vector<float> out;

    m.input("signal").set(in_signal);
    m.output("signal").set(out);
    m.compute();

    std::vector<float> expected{0.166667, 0.333333, 0.5, 0.666667, 0.833333, 1, 1, 1, 1, 1};
    ASSERT_THAT(out, Pointwise(NearWithPrecision(1e-5), expected));
}

TEST_F(AMovingAverage, ResetClearInternalMean) {
    m.configure();
    std::vector<float> in_signal(10, 1.0f);
    std::vector<float> out;
    m.input("signal").set(in_signal);
    m.output("signal").set(out);
    m.compute();

    m.reset();

    ASSERT_THAT(m.getPreMean(), Eq(0.0f));
}