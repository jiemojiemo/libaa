//
// Created by user on 5/9/22.
//
#include "libaa/graph/aa_parameter_change_port.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AParameterChangePort : public Test {
public:
    void SetUp() override {
        block = std::make_shared<AudioBlock>(0, 0, num_params);
    }
    std::shared_ptr<AudioBlock> block;
    int num_params = 10;
};

TEST_F(AParameterChangePort, CanInitWithBlock) {
    auto port = std::make_unique<ParameterChangePort>(block);
}

TEST_F(AParameterChangePort, CanGetParameterChanges) {
    auto port = std::make_unique<ParameterChangePort>(block);

    ASSERT_THAT(port->getParameterChanges().getNumParameters(), Eq(num_params));
}

TEST_F(AParameterChangePort, CanPushParameterChangePoint) {
    auto port = std::make_unique<ParameterChangePort>(block);

    port->pushParameterChange({0, 0, 1});

    ParameterChangePoint p{};
    port->getParameterChanges().pop(0, p);

    ASSERT_THAT(p.time, Eq(0));
    ASSERT_THAT(p.normalized_value, Eq(1));
}