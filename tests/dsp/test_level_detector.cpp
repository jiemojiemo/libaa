//
// Created by hw on 2021/12/27.
//

#include "libaa/dsp/aa_level_detector.h"
#include <gmock/gmock.h>
using namespace libaa;
using namespace testing;

class ALevelDetector : public Test {
public:
    LevelDetectorParameters p;
    float sample_rate = 44100;
};

TEST_F(ALevelDetector, CanConstructWithParameters) {
    LevelDetector detector(p);
}

TEST_F(ALevelDetector, CanGetSampleRateAfterPrepare) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    ASSERT_THAT(detector.getSampleRate(), Eq(sample_rate));
}

TEST_F(ALevelDetector, CanSetAttackTime) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float new_attack_time_ms = 50;
    detector.setAttackTime(new_attack_time_ms);

    ASSERT_THAT(detector.getAttackTime(), FloatEq(new_attack_time_ms));
}

TEST_F(ALevelDetector, CanSetReleaseTime) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float new_release_time_ms = 50;
    detector.setReleaseTime(new_release_time_ms);

    ASSERT_THAT(detector.getReleaseTime(), FloatEq(new_release_time_ms));
}

TEST_F(ALevelDetector, CanGetAttackTimeAlphaAfterConstruct) {
    p.attack_time_ms = 50;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float expected_alpha =
        exp(-1.0f / (p.attack_time_ms * sample_rate * 0.001));
    ASSERT_THAT(detector.getAttackAlpha(), FloatEq(expected_alpha));
}

TEST_F(ALevelDetector, CanGetReleaseTimeAlphaAfterConstruct) {
    p.release_time_ms = 20;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float expected_alpha =
        exp(-1.0f / (p.release_time_ms * sample_rate * 0.001));
    ASSERT_THAT(detector.getReleaseAlpha(), FloatEq(expected_alpha));
}

TEST_F(ALevelDetector, AttackTimeAlphaUpdatedAfterSetAttackTime) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float new_attack_time_ms = 50;
    detector.setAttackTime(new_attack_time_ms);

    float expected_alpha =
        exp(-1.0f / (new_attack_time_ms * sample_rate * 0.001));
    ASSERT_THAT(detector.getAttackAlpha(), FloatEq(expected_alpha));
}

TEST_F(ALevelDetector, ReleaseTimeAlphaUpdatedAfterSetReleaseTime) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float new_release_time_ms = 20;
    detector.setReleaseTime(new_release_time_ms);

    float expected_alpha =
        exp(-1.0f / (new_release_time_ms * sample_rate * 0.001));
    ASSERT_THAT(detector.getReleaseAlpha(), FloatEq(expected_alpha));
}

TEST_F(ALevelDetector, CanUpdateParameters) {
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    p.attack_time_ms = 50;
    p.release_time_ms = 20;
    p.detect_db = true;
    p.clamp_to_unity_max = true;

    detector.setParameters(p);

    ASSERT_THAT(detector.getParameters(), Eq(p));
}

TEST_F(ALevelDetector, CanProcessAndReturnsWithModeMS) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_MS;
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 0.5f;
    float expected = 0.5 * (xn * xn);
    ASSERT_THAT(detector.process(xn), Eq(expected));
}

TEST_F(ALevelDetector, CanProcessAndReturnsWithModeRMS) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_RMS;
    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 0.5f;
    float expected = powf(0.5 * (xn * xn), 0.5f);
    ASSERT_THAT(detector.process(xn), Eq(expected));
}

TEST_F(ALevelDetector, CanProcessAndReturnWithPeakMode) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_Peak;
    p.release_time_ms = 50;
    p.attack_time_ms = 10;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 0.5f;
    float y1 = std::max(xn, detector.getReleaseAlpha() * 0 +
                                (1 - detector.getReleaseAlpha()) * xn);
    float expected =
        detector.getAttackAlpha() * 0 + (1 - detector.getAttackAlpha()) * y1;

    ASSERT_THAT(detector.process(xn), Eq(expected));
}

TEST_F(ALevelDetector, CanProcessWithClampToMax) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_Peak;
    p.clamp_to_unity_max = true;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 2.0f;
    float expected = 1.0f;

    ASSERT_THAT(detector.process(xn), Eq(expected));
}

TEST_F(ALevelDetector, CanProcessAndReturnDb) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_RMS;
    p.detect_db = true;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 0.5f;
    float expected = powf(0.5 * (xn * xn), 0.5f);
    expected = 20 * log10(expected);
    ASSERT_THAT(detector.process(xn), Eq(expected));
}

TEST_F(ALevelDetector, ReturnMinDbIfInputIsZero) {
    p.mode = LevelDetectorMode::kLevelDetectorMode_RMS;
    p.detect_db = true;

    LevelDetector detector(p);
    detector.prepare(sample_rate);

    float xn = 0.0;
    float expected = -96.0f;
    ASSERT_THAT(detector.process(xn), Eq(expected));
}