//
// Created by William on 3/28/22.
//
#include "ae_demo_context.h"
#include "ae_demo_pipeline.h"

#include <gmock/gmock.h>
#include <string>
#include <utility>
using namespace testing;
using namespace std;

class ChangeDemoNameValue : public DemoValue {
public:
    virtual ~ChangeDemoNameValue() = default;

    ChangeDemoNameValue(std::string name) : new_name(std::move(name)) {}

    void process(DemoContext &context) override {
        context.demo_name = new_name;
    }

    std::string new_name;
};

class ADemoPipeline : public Test {
public:
    DemoContext context;
};

TEST_F(ADemoPipeline, CanAddValueToLast) {
    DemoPipeline pipeline;

    pipeline.addLast(new DemoValue);

    ASSERT_THAT(pipeline.values.size(), Eq(1));
}

TEST_F(ADemoPipeline, CanStartPipeline) {
    DemoPipeline pipeline;
    string new_name = "abc";
    pipeline.addLast(new ChangeDemoNameValue(new_name));

    pipeline.start(context);

    ASSERT_THAT(context.demo_name, Eq(new_name));
}