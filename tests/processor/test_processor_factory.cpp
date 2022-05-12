//
// Created by user on 5/12/22.
//
#include "libaa/processor/aa_processor_factory.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AProcessorFactory : public Test {
public:
};

TEST_F(AProcessorFactory, CanCreateGainProcessor) {
    ASSERT_THAT(ProcessorFactory::create("Gain"), NotNull());
}

TEST_F(AProcessorFactory, CanCreateDelayProcessor) {
    ASSERT_THAT(ProcessorFactory::create("Delay"), NotNull());
}

TEST_F(AProcessorFactory, CanCreatePitchShiftProcessor) {
    ASSERT_THAT(ProcessorFactory::create("PitchShifter"), NotNull());
}

TEST_F(AProcessorFactory, CanCreateReverbTankProcessor) {
    ASSERT_THAT(ProcessorFactory::create("ReverbTank"), NotNull());
}

TEST_F(AProcessorFactory, CanCreateSourceProcessor) {
    ASSERT_THAT(ProcessorFactory::create("Source"), NotNull());
}

TEST_F(AProcessorFactory, CanCreateSourceCallbackProcessor) {
    ASSERT_THAT(ProcessorFactory::create("Source Callback"), NotNull());
}

TEST_F(AProcessorFactory, CanCreateVibratoProcessor) {
    ASSERT_THAT(ProcessorFactory::create("Vibrato"), NotNull());
}