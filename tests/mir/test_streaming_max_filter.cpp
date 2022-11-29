//
// Created by user on 6/29/22.
//
#include "libaa/mir/aa_streaming_max_filter.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
using namespace libaa::MIR;

class AStreamingMaxFilter : public Test {
public:
    StreamingMaxFilter f;
};

TEST_F(AStreamingMaxFilter, CanReportCorrectName) {
    ASSERT_THAT(f.getName(), Eq("StreamingMaxFilter"));
}

TEST_F(AStreamingMaxFilter, SignalInputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(f.input("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingMaxFilter, SignalOutputAsExpected) {
    const std::type_info &expected = typeid(float);

    ASSERT_THAT(f.output("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AStreamingMaxFilter, WidthParameterAsExpected) {
    auto &p = f.getParameter("width");

    ASSERT_THAT(p.getParameterName(), Eq("width"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(3));
    ASSERT_THAT(p.getMinPlainValue(), Eq(2));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AStreamingMaxFilter, ConfigureCreateSimpleRingBuffer) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    ASSERT_THAT(f.getSimpleRingBuffer(), NotNull());
}

TEST_F(AStreamingMaxFilter, ConfigureCreateSimpleRingBufferWithWidthSize) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    auto expected = f.getParameter("width").getInt();
    ASSERT_THAT(f.getSimpleRingBuffer()->data.size(), Eq(expected));
}

TEST_F(AStreamingMaxFilter, ConfigureCreateSimpleRingBufferMinimumFloatValue) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    ASSERT_THAT(f.getSimpleRingBuffer()->data[0], Eq(std::numeric_limits<float>::lowest()));
}

TEST_F(AStreamingMaxFilter, ComputeGetMaxFilterResult) {
    std::vector<float> test_signal{1, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    std::vector<float> expected{1, 1, 1, 1, 0, 1, 1, 1, 0, 0};
    float in;
    float out;
    f.input("signal").set(in);
    f.output("signal").set(out);
    f.configure();

    for (int i = 0; i < test_signal.size(); ++i) {
        in = test_signal[i];
        f.compute();

        ASSERT_THAT(out, FloatEq(expected[i]));
    }
}
