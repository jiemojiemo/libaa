//
// Created by user on 6/24/22.
//
#include "libaa/mir/aa_max_filter.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;
class AMaxFilter : public Test {
public:
    MaxFilter f;
};

TEST_F(AMaxFilter, CanReportCorrectName) {
    ASSERT_THAT(f.getName(), Eq("MaxFilter"));
}

TEST_F(AMaxFilter, SignalInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(f.input("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMaxFilter, SignalOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(f.output("signal").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AMaxFilter, WidthParameterAsExpected) {
    auto &p = f.getParameter("width");

    ASSERT_THAT(p.getParameterName(), Eq("width"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(3));
    ASSERT_THAT(p.getMinPlainValue(), Eq(2));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(AMaxFilter, ConfigureCreateSimpleRingBuffer) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    ASSERT_THAT(f.getSimpleRingBuffer(), NotNull());
}

TEST_F(AMaxFilter, ConfigureCreateSimpleRingBufferWithWidthSize) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    auto expected = f.getParameter("width").getInt();
    ASSERT_THAT(f.getSimpleRingBuffer()->data.size(), Eq(expected));
}

TEST_F(AMaxFilter, ConfigureCreateSimpleRingBufferMinimumFloatValue) {
    ASSERT_THAT(f.getSimpleRingBuffer(), IsNull());

    f.configure();

    ASSERT_THAT(f.getSimpleRingBuffer()->data[0], Eq(std::numeric_limits<float>::lowest()));
}

TEST_F(AMaxFilter, ComputeGetMaxFilterResult) {
    std::vector<float> in{1, 1, 0, 0, 0, 1, 0, 0, 0, 0};
    std::vector<float> out;
    f.input("signal").set(in);
    f.output("signal").set(out);
    f.configure();

    f.compute();

    std::vector<float> expected{1, 1, 1, 1, 0, 1, 1, 1, 0, 0};
    ASSERT_THAT(out.size(), Eq(in.size()));
    ASSERT_THAT(out, ContainerEq(expected));
}

class AMaxFilterSimpleRingbuffer : public Test {
public:
    size_t size = 3;
    float v = 1.0;
};

TEST_F(AMaxFilterSimpleRingbuffer, CanInitWithSizeAndInitValue) {
    MaxFilter::SimpleRingBuffer buffer(size, v);

    ASSERT_THAT(buffer.data.size(), Eq(size));
    ASSERT_THAT(buffer.data[0], Eq(1.0));
}

TEST_F(AMaxFilterSimpleRingbuffer, CanPushValueToBuffer) {
    MaxFilter::SimpleRingBuffer buffer(size, v);

    buffer.push(2.0);

    ASSERT_THAT(buffer.data, Contains(2.0));
}

TEST_F(AMaxFilterSimpleRingbuffer, WriteIndexIncreasedAfterPush) {
    MaxFilter::SimpleRingBuffer buffer(size, v);

    ASSERT_THAT(buffer.write_index, Eq(0));
    buffer.push(0);

    ASSERT_THAT(buffer.write_index, Eq(1));
}

TEST_F(AMaxFilterSimpleRingbuffer, WriteIndexLoopBack) {
    MaxFilter::SimpleRingBuffer buffer(size, v);

    for (int i = 0; i < size; ++i) {
        buffer.push(0);
    }

    ASSERT_THAT(buffer.write_index, Eq(0));
}