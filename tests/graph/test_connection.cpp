//
// Created by user on 5/9/22.
//
#include "libaa/graph/aa_connection.h"
#include "libaa_testing/aa_mock_node.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AConnection : public Test {
public:
    void SetUp() override {
        c = std::make_unique<Connection>(n, up_port_index,
                                         down_port_index);
    }
    std::shared_ptr<MockNode> n = std::make_shared<MockNode>();
    std::unique_ptr<Connection> c;

    const int up_port_index = 1;
    const int down_port_index = 2;
};

TEST_F(AConnection, CanInitWithNodeAndPortIndex) {
    auto c = std::make_unique<Connection>(n, up_port_index, down_port_index);
}

TEST_F(AConnection, CanGetUpStreamNode) {
    ASSERT_THAT(c->upstream_node.get(), Eq(n.get()));
}

TEST_F(AConnection, CanGetUpStreamPortIndex) {
    ASSERT_THAT(c->upstream_port_index, Eq(up_port_index));
}

TEST_F(AConnection, CanGetDownStreamPortIndex) {
    ASSERT_THAT(c->downstream_port_index, Eq(down_port_index));
}