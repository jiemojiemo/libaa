//
// Created by William on 3/23/22.
//

#include "libaa/core/aa_parameter_changes.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AParameterChanges : public Test {
public:
    size_t num_param = 10;
    size_t fifo_size = 1;
    ParameterChangePoint p0{0, 0, 1};
    ParameterChangePoint p1{1, 0, 1};
};

TEST_F(AParameterChanges, ConstructWithSizes) {
    ParameterChanges param_changes(num_param, fifo_size);
}

TEST_F(AParameterChanges, CanGetNumberParameter) {
    ParameterChanges param_changes(num_param, fifo_size);

    ASSERT_THAT(param_changes.getNumParameters(), Eq(num_param));
}

TEST_F(AParameterChanges, CanPushParameterChangePoint) {
    ParameterChanges param_changes(num_param, fifo_size);

    bool ok = param_changes.push(0, p0);

    ASSERT_TRUE(ok);
}

TEST_F(AParameterChanges, PushFailedIfParamIndexOutOfSize) {
    int outof_size_index = num_param;
    ParameterChanges param_changes(num_param, fifo_size);

    bool ok = param_changes.push(outof_size_index, p0);

    ASSERT_FALSE(ok);
}

TEST_F(AParameterChanges, CanPopParameterChangePoint) {
    ParameterChanges param_changes(num_param, fifo_size);
    param_changes.push(0, p0);
    param_changes.push(1, p1);

    ParameterChangePoint output_point;
    bool ok = param_changes.pop(0, output_point);
    ASSERT_TRUE(ok);
    ASSERT_THAT(output_point.index, Eq(0));
    ASSERT_THAT(output_point.time, Eq(0));
    ASSERT_THAT(output_point.normalized_value, Eq(1));

    ok = param_changes.pop(1, output_point);
    ASSERT_TRUE(ok);
    ASSERT_THAT(output_point.index, Eq(1));
    ASSERT_THAT(output_point.time, Eq(0));
    ASSERT_THAT(output_point.normalized_value, Eq(1));
}

TEST_F(AParameterChanges, PopFailedIfParamIndexOutOfSize) {
    int outof_size_index = num_param;
    ParameterChanges param_changes(num_param, fifo_size);
    param_changes.push(0, p0);

    bool ok = param_changes.pop(outof_size_index, p0);

    ASSERT_FALSE(ok);
}