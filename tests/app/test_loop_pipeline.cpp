//
// Created by William on 3/30/22.
//
#include "ae_demo_pipeline.h"
#include "ae_loop_begin_value.h"
#include "ae_loop_end_value.h"
#include <gmock/gmock.h>
using namespace testing;

class MockCounterValue : public DemoValue {
public:
    void process(DemoContext &context) override {
        counter += 1;
    }
    int counter = 0;
};

TEST(LoopPipeline, BuildWithLoopBeginAndLoopEndValue) {
    DemoPipeline pipeline;
    auto *begin_value = new LoopBeginValue;
    auto *end_value = new LoopEndValue(begin_value);
    auto *mock_value = new MockCounterValue;
    pipeline.addLast(begin_value);
    pipeline.addLast(mock_value);
    pipeline.addLast(end_value);
    DemoContext context;
    context.num_round = 5;

    pipeline.start(context);

    ASSERT_THAT(mock_value->counter, Eq(context.num_round));
}