
//
// Created by William.Hua on 2021/7/26.
//

#include "aa_test_helper.h"
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class AParameterChange : public Test {
public:
    ParameterChangePoint point{0, 0, 0};
};

TEST_F(AParameterChange, CanInitWithSize) {
    size_t p_size = 2;
    ParameterChange param_change(p_size);

    ASSERT_THAT(param_change.getSize(), Eq(p_size));
}

TEST_F(AParameterChange, CanPushPoint) {
    ParameterChange param_change(2);
    auto input_point = ParameterChangePoint{0, 0, 0.1};

    bool ok = param_change.push(input_point);
    ASSERT_TRUE(ok);
}

TEST_F(AParameterChange, CanPopPoint) {
    ParameterChange param_change(2);
    auto input_point = ParameterChangePoint{1, 1, 1};
    param_change.push(input_point);

    bool ok = param_change.pop(point);

    ASSERT_TRUE(ok);
    ASSERT_THAT(point, Eq(input_point));
}

TEST_F(AParameterChange, PushOverWriteIfOutOfSize) {
    ParameterChange param_change(2);
    param_change.push({1, 1, 1});
    param_change.push({2, 2, 2});
    param_change.push({3, 3, 3});

    bool ok = param_change.pop(point);

    ASSERT_TRUE(ok);
    ASSERT_THAT(point, Eq(ParameterChangePoint{3, 3, 3}));
}