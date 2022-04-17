//
// Created by user on 4/15/22.
//
#include "libaa/dsp/aa_simple_delay.h"
#include <gmock/gmock.h>
#include <memory>
using namespace testing;
using namespace std;
using namespace libaa;

class ASimpleDelay : public Test {
public:
    SimpleDelay d;
    int sample_rate = 10;
    const int delay_line_size = 128;
};

TEST_F(ASimpleDelay, DefaultDelaySizeIs0) {
    ASSERT_THAT(d.getSize(), Eq(0));
}

TEST_F(ASimpleDelay, ResizeWillChangeSize) {
    d.resize(delay_line_size);

    ASSERT_THAT(d.getSize(), Eq(delay_line_size));
}

TEST_F(ASimpleDelay, NeedPrepareBeforeProcess) {
    d.prepare(sample_rate);
}

TEST_F(ASimpleDelay, ConstructWithSize) {
    auto d = std::make_unique<SimpleDelay>(delay_line_size);

    ASSERT_THAT(d->getSize(), Eq(delay_line_size));
}

TEST_F(ASimpleDelay, CanUpdateParameters) {
    SimpleDelay::SimpleDelayParameters parameters{};
    parameters.delay_ms = 1.0f;
    parameters.interpolate = true;

    d.updateParameters(parameters);

    ASSERT_THAT(d.getParameters().delay_ms, Eq(parameters.delay_ms));
    ASSERT_THAT(d.getParameters().interpolate, Eq(parameters.interpolate));
}

TEST_F(ASimpleDelay, CanReadDelaySamples) {
    sample_rate = 10;
    d.resize(delay_line_size);
    d.prepare(sample_rate);
    SimpleDelay::SimpleDelayParameters parameters{};
    parameters.delay_ms = 300.0f;
    parameters.interpolate = false;
    d.updateParameters(parameters);

    d.processSample(1);
    d.processSample(2);
    d.processSample(3);
    d.processSample(4);

    ASSERT_THAT(d.readDelay(), Eq(1));
}

TEST_F(ASimpleDelay, CanReadAtMs) {
    sample_rate = 2;
    d.resize(delay_line_size);
    d.prepare(sample_rate);
    SimpleDelay::SimpleDelayParameters parameters{};
    parameters.delay_ms = 1000.0f;
    d.updateParameters(parameters);

    d.writeSample(1);
    d.writeSample(2);
    d.writeSample(3);
    d.writeSample(4);

    ASSERT_THAT(d.readDelayAt(1000), Eq(2));
}

TEST_F(ASimpleDelay, CanWriteSample) {
    sample_rate = 10;
    d.resize(delay_line_size);
    d.prepare(sample_rate);
    SimpleDelay::SimpleDelayParameters parameters{};
    parameters.delay_ms = 300.0f;
    parameters.interpolate = false;
    d.updateParameters(parameters);

    d.writeSample(1);
    d.writeSample(2);
    d.writeSample(3);
    d.writeSample(4);

    ASSERT_THAT(d.readDelay(), Eq(1));
}

TEST_F(ASimpleDelay, ProcessGetDelaySample) {
    sample_rate = 10;
    d.resize(delay_line_size);
    d.prepare(sample_rate);
    SimpleDelay::SimpleDelayParameters parameters{};
    parameters.delay_ms = 300.0f;
    parameters.interpolate = false;
    d.updateParameters(parameters);

    ASSERT_THAT(d.processSample(1), Eq(0));
    ASSERT_THAT(d.processSample(2), Eq(0));
    ASSERT_THAT(d.processSample(3), Eq(0));
    ASSERT_THAT(d.processSample(4), Eq(1));
    ASSERT_THAT(d.processSample(5), Eq(2));
}
