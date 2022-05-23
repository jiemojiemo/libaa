//
// Created by user on 5/19/22.
//
#include "libaa/processor/aa_dynamic_processor_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ADynamicProcessorUtilities : public Test {
public:
    DynamicProcessorUtilities u;
    DynamicProcessorParameters parameters;
    float sample_rate = 44100;
};

TEST_F(ADynamicProcessorUtilities, CanUpdateParameters) {
    parameters.release_time_ms = 100;
    parameters.attack_time_ms = 50;

    u.updateParameters(parameters);

    ASSERT_THAT(u.getParameters().release_time_ms, Eq(100));
    ASSERT_THAT(u.getParameters().attack_time_ms, Eq(50));
}

TEST_F(ADynamicProcessorUtilities, SetReleaseTimeWillUpdateLevelDetectorReleaseTime) {
    parameters.release_time_ms = 10;
    u.updateParameters(parameters);

    ASSERT_THAT(u.detector.getReleaseTime(), Eq(parameters.release_time_ms));
}

TEST_F(ADynamicProcessorUtilities, SetAttackTimeWillUpdateLevelDetectorAttackTime) {
    parameters.attack_time_ms = 10;
    u.updateParameters(parameters);

    ASSERT_THAT(u.detector.getAttackTime(), Eq(parameters.attack_time_ms));
}

TEST_F(ADynamicProcessorUtilities, PrepareWillPrepareLevelDetector) {
    u.prepare(sample_rate);

    ASSERT_THAT(u.detector.getSampleRate(), Eq(sample_rate));
}

TEST_F(ADynamicProcessorUtilities, CanComputeGainIfHardKnee) {
    parameters.soft_knee = false;
    parameters.threshold_db = -10;
    parameters.ratio = 5;
    u.updateParameters(parameters);

    float below_threshold = -20;
    float expected = below_threshold;
    ASSERT_THAT(u.computeGain(below_threshold), Eq(expected));

    float above_threshold = -5;
    expected = parameters.threshold_db + (above_threshold - parameters.threshold_db) / parameters.ratio;
    ASSERT_THAT(u.computeGain(above_threshold), Eq(expected));
}

TEST_F(ADynamicProcessorUtilities, CanComputeGainIfSoftKnee) {
    parameters.soft_knee = true;
    parameters.threshold_db = -10;
    parameters.ratio = 5;
    parameters.knee_width = 10;
    u.updateParameters(parameters);

    float left_part_input = -20;
    float expected = left_part_input;
    ASSERT_THAT(u.computeGain(left_part_input), Eq(expected));

    float right_part_input = -1;
    expected = parameters.threshold_db + (right_part_input - parameters.threshold_db) / parameters.ratio;
    ASSERT_THAT(u.computeGain(right_part_input), Eq(expected));

    float middle_part_input = -10;
    float a = 1.0f / parameters.ratio - 1;
    float b = (middle_part_input - parameters.threshold_db + parameters.knee_width / 2);
    float c = b * b;
    expected = middle_part_input + (a * c) / (2 * parameters.knee_width);
    ASSERT_THAT(u.computeGain(middle_part_input), Eq(expected));
}

TEST_F(ADynamicProcessorUtilities, CanComputeGainIfIsLimiterMode) {
    parameters.threshold_db = -10;
    parameters.hard_limiter = true;
    u.updateParameters(parameters);

    float input = -5;
    float expected = parameters.threshold_db;
    ASSERT_THAT(u.computeGain(input), Eq(expected));
}

TEST_F(ADynamicProcessorUtilities, CanProcessAsExpected) {
    parameters.threshold_db = -10;
    parameters.knee_width = 10;
    parameters.ratio = 5;
    parameters.attack_time_ms = 1;
    parameters.release_time_ms = 1;
    parameters.output_gain_db = 1;
    parameters.soft_knee = false;
    u.updateParameters(parameters);
    u.prepare(sample_rate);

    float in = 0.8f;
    float expected = 0.882795f;
    ASSERT_THAT(u.process(in), FloatNear(expected, 1e-6));
}