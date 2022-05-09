//
// Created by user on 5/9/22.
//
#include "libaa/graph/aa_parameter_change_connection.h"
#include "libaa_testing/aa_mock_node.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AParameterChangeConnection : public Test {
public:
    void SetUp() override {
        block = std::make_shared<AudioBlock>(0, 0, num_params);
        c = std::make_unique<ParameterChangeConnection>(n, up_port_index,
                                                        down_port_index);
    }
    std::shared_ptr<MockNode> n = std::make_shared<MockNode>();
    std::unique_ptr<ParameterChangeConnection> c;
    std::shared_ptr<AudioBlock> block;

    const int num_params = 10;
    const int up_port_index = 1;
    const int down_port_index = 2;
};

TEST_F(AParameterChangeConnection, InitWithUpstreamNodeAndPort) {
    c = std::make_unique<ParameterChangeConnection>(
        n, 0, 0);

    ASSERT_THAT(c->upstream_node.get(), Eq(n.get()));
}

TEST_F(AParameterChangeConnection, CanGetUpStreamNode) {
    ASSERT_THAT(c->upstream_node.get(), Eq(n.get()));
}

TEST_F(AParameterChangeConnection, CanGetUpStreamPortIndex) {
    ASSERT_THAT(c->upstream_param_change_port_index, Eq(up_port_index));
}

TEST_F(AParameterChangeConnection, CanGetDownStreamPortIndex) {
    ASSERT_THAT(c->downstream_param_change_port_index, Eq(down_port_index));
}

TEST_F(AParameterChangeConnection, PullWillTriggerUpstreamNodeToPull) {
    ParameterChangePort pc_port(block);

    EXPECT_CALL(*n, pullParameterChangePort(up_port_index)).WillOnce(ReturnRef(pc_port));

    ParameterChangePort &output_p = c->pull();

    ASSERT_THAT(output_p.getParameterChanges().getNumParameters(), Eq(num_params));
}