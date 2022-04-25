//
// Created by user on 4/24/22.
//

#include "libaa/processor/aa_pitch_shifter_utilities.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;
using namespace libaa;

class APitchShiftUtilities : public Test {
public:
    int sample_rate = 44100;
    int num_channels = 2;
};

TEST_F(APitchShiftUtilities, InitBlockSizeIs1024) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    ASSERT_THAT(p->m_blockSize, Eq(1024));
}

TEST_F(APitchShiftUtilities, Has8192Latency) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    ASSERT_THAT(p->getLatency(), Eq(8192));
}

TEST_F(APitchShiftUtilities, UpdateRatioWithOctaveHasRange) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float semitones = 0;
    float cents = 0;
    p->m_semitones = &semitones;
    p->m_cents = &cents;

    float o0 = 0.5;
    float o1 = -3;
    float o2 = 3;
    p->m_octaves = &o0;
    p->updateRatio();
    double expect = pow(2.0, round(o0) + semitones / 12.0 + cents / 1200.0);

    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_octaves = &o1;
    p->updateRatio();
    expect = pow(2.0, round(-2) + semitones / 12.0 + cents / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_octaves = &o2;
    p->updateRatio();
    expect = pow(2.0, round(2) + semitones / 12.0 + cents / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));
}

TEST_F(APitchShiftUtilities, UpdateRatioWithSemitonesHasRange) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float octaves = 0;
    float cents = 0;
    p->m_octaves = &octaves;
    p->m_cents = &cents;

    float s0 = 5;
    float s1 = -13;
    float s2 = 13;
    p->m_semitones = &s0;
    p->updateRatio();
    double expect = pow(2.0, round(octaves) + s0 / 12.0 + cents / 1200.0);

    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_semitones = &s1;
    p->updateRatio();
    expect = pow(2.0, round(octaves) + -12.0 / 12.0 + cents / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_semitones = &s2;
    p->updateRatio();
    expect = pow(2.0, round(octaves) + 12.0 / 12.0 + cents / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));
}

TEST_F(APitchShiftUtilities, UpdateRatioWithCentsHasRange) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float octaves = 0;
    float semitones = 0;
    p->m_semitones = &semitones;
    p->m_octaves = &octaves;

    float s0 = 50;
    float s1 = -130;
    float s2 = 130;
    p->m_cents = &s0;
    p->updateRatio();
    double expect = pow(2.0, round(octaves) + semitones / 12.0 + s0 / 1200.0);

    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_cents = &s1;
    p->updateRatio();
    expect = pow(2.0, round(octaves) + semitones / 12.0 + -100 / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));

    p->m_cents = &s2;
    p->updateRatio();
    expect = pow(2.0, round(octaves) + semitones / 12.0 + 100 / 1200.0);
    ASSERT_THAT(p->m_ratio, DoubleEq(expect));
}

TEST_F(APitchShiftUtilities, HasDefaultCrispness) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);
    ASSERT_THAT(p->m_currentCrispness, Eq(-1));
}

TEST_F(APitchShiftUtilities, CanUpdateCrispness) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float crispness = 0;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(int(crispness)));

    crispness = 0;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(int(crispness)));

    crispness = 1;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(int(crispness)));

    crispness = 2;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(int(crispness)));

    crispness = 3;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(int(crispness)));
}

TEST_F(APitchShiftUtilities, CrispnessHasRange) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float crispness = -1;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(-1));

    crispness = 4;
    p->m_crispness = &crispness;
    p->updateCrispness();
    ASSERT_THAT(p->m_currentCrispness, Eq(-1));
}

TEST_F(APitchShiftUtilities, HasDefaultFormat) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    ASSERT_FALSE(p->m_currentFormant);
}

TEST_F(APitchShiftUtilities, CanUpdateFormat) {
    auto p = std::make_unique<PitchShifterUtilities>(sample_rate, num_channels);

    float format = 1.0f;
    p->m_formant = &format;
    p->updateFormant();
    ASSERT_TRUE(p->m_currentFormant);

    format = 0.0f;
    p->m_formant = &format;
    p->updateFormant();
    ASSERT_FALSE(p->m_currentFormant);
}