//
// Created by user on 4/19/22.
//

#include "libaa/processor/aa_reverb_tank_utilities.h"
#include <gmock/gmock.h>

using namespace testing;

using namespace libaa;
class AReverbTankProcessorUtilities : public Test {
public:
    ReverbTankUtilities u;
    float sample_rate = 44100;
    const int max_block_size = 1024;
    int num_params = u.params.size();
    AudioBlock block{{{1, 0, 0, 0}, {0, 0, 0, 0}}, num_params};
};

TEST_F(AReverbTankProcessorUtilities, HasFourBranch) {
    ASSERT_THAT(u.kNumBranch, Eq(4));
    ASSERT_THAT(u.branch_nested_apfs.size(), Eq(4));
    ASSERT_THAT(u.branch_delays.size(), Eq(4));
    ASSERT_THAT(u.branch_lpfs.size(), Eq(4));
}

TEST_F(AReverbTankProcessorUtilities, HasTwoShelvingFilters) {
    ASSERT_THAT(u.shelving_filters.size(), Eq(2));
}

TEST_F(AReverbTankProcessorUtilities, PrepareWillSetDefaultPreDelayParameters) {
    auto expected =
        u.params.get(ReverbTankUtilities::kPreDelay).getPlainValue();

    u.prepareToPlay(sample_rate, max_block_size);

    ASSERT_THAT(u.pre_delay.getParameters().delay_ms, Eq(expected));
}

TEST_F(AReverbTankProcessorUtilities,
       PrepareWillSetDefaultBranchDelaysParameters) {
    auto max_fix_delay =
        u.params.get(ReverbTankUtilities::kMaxFixedDelay).getPlainValue();
    auto weight =
        u.params.get(ReverbTankUtilities::kFixedDelayWeight).getPlainValue();

    u.prepareToPlay(sample_rate, max_block_size);

    for (int i = 0; i < u.branch_delays.size(); ++i) {
        auto expected = max_fix_delay * weight * u.fixed_delay_weight[i];
        ASSERT_THAT(u.branch_delays[i].getParameters().delay_ms, Eq(expected));
    }
}

TEST_F(AReverbTankProcessorUtilities,
       PrepareWillSetDefaultBranchNestedAPFsParameters) {
    auto max_apf_delay =
        u.params.get(ReverbTankUtilities::kMaxAPFDelay).getPlainValue();
    auto weight =
        u.params.get(ReverbTankUtilities::kAPFDelayWeight).getPlainValue();

    u.prepareToPlay(sample_rate, max_block_size);

    NestedDelayAPF::NestedDelayAPFParameters expected;
    int m = 0;
    for (int i = 0; i < u.branch_nested_apfs.size(); ++i) {
        expected.delay_ms = max_apf_delay * weight * u.apf_delay_weight[m++];
        expected.inner_apf_delay_ms =
            max_apf_delay * weight * u.apf_delay_weight[m++];
        expected.enable_LFO = true;
        expected.lfo_depth = 1.0f;
        expected.lfo_max_modulation_ms = 0.3;
        expected.apf_g = 0.5;
        expected.inner_apf_g = -0.5;
        switch (i) {
        case 0:
            expected.lfo_rate_hz = 0.15;
            break;
        case 1:
            expected.lfo_rate_hz = 0.33;
            break;
        case 2:
            expected.lfo_rate_hz = 0.57;
            break;
        case 3:
            expected.lfo_rate_hz = 0.73;
            break;
        }

        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().delay_ms,
                    Eq(expected.delay_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().inner_apf_delay_ms,
                    Eq(expected.inner_apf_delay_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().enable_LFO,
                    Eq(expected.enable_LFO));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().lfo_depth,
                    Eq(expected.lfo_depth));
        ASSERT_THAT(
            u.branch_nested_apfs[i].getParameters().lfo_max_modulation_ms,
            Eq(expected.lfo_max_modulation_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().lfo_rate_hz,
                    Eq(expected.lfo_rate_hz));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().apf_g,
                    Eq(expected.apf_g));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().inner_apf_g,
                    Eq(expected.inner_apf_g));
    }
}

TEST_F(AReverbTankProcessorUtilities,
       PrepareWillSetDefaultBranchLPFsParameters) {
    auto expected = u.params.get(ReverbTankUtilities::kDamping).getPlainValue();

    u.prepareToPlay(sample_rate, max_block_size);

    for (int i = 0; i < u.branch_lpfs.size(); ++i) {
        ASSERT_THAT(u.branch_lpfs[i].getParameters().lpf_g, Eq(expected));
    }
}

TEST_F(AReverbTankProcessorUtilities,
       PrepareWillSetDefaultLowShelvingFiltersParameters) {
    auto low_fc =
        u.params.get(ReverbTankUtilities::kLowShelfFc).getPlainValue();
    auto low_db =
        u.params.get(ReverbTankUtilities::kLowShelfGainDB).getPlainValue();
    auto high_fc =
        u.params.get(ReverbTankUtilities::kHighShelfFc).getPlainValue();
    auto high_db =
        u.params.get(ReverbTankUtilities::kHighShelfGainDB).getPlainValue();

    u.prepareToPlay(sample_rate, max_block_size);

    for (int i = 0; i < u.shelving_filters.size(); ++i) {
        ASSERT_THAT(u.shelving_filters[i].getParameters().low_shelf_fc,
                    Eq(low_fc));
        ASSERT_THAT(u.shelving_filters[i].getParameters().low_shelf_boost_db,
                    Eq(low_db));
        ASSERT_THAT(u.shelving_filters[i].getParameters().high_shelf_fc,
                    Eq(high_fc));
        ASSERT_THAT(u.shelving_filters[i].getParameters().high_shelf_boost_db,
                    Eq(high_db));
    }
}

TEST_F(AReverbTankProcessorUtilities, ProcessUpdatePreDelayParameters) {
    float norm_v = 0.5;
    AudioBlock block{{{1, 0, 0, 0}, {0, 0, 0, 0}}, num_params};
    block.param_changes.push(ReverbTankUtilities::kPreDelay, {0, 0, norm_v});

    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    float expected = u.params.get(ReverbTankUtilities::kPreDelay)
                         .convertNormalizedValueToPlainValue(norm_v);
    ASSERT_THAT(
        u.params.get(ReverbTankUtilities::kPreDelay).getNormalizedValue(),
        Eq(norm_v));
    ASSERT_THAT(u.pre_delay.getParameters().delay_ms, Eq(expected));
}

TEST_F(AReverbTankProcessorUtilities, ProcessUpdateBranchAPFsParameters) {
    float norm_v = 0.5;
    block.param_changes.push(ReverbTankUtilities::kMaxAPFDelay, {0, 0, norm_v});
    block.param_changes.push(ReverbTankUtilities::kAPFDelayWeight,
                             {0, 0, norm_v});

    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    float max_apf_delay = u.params.get(ReverbTankUtilities::kMaxAPFDelay)
                              .convertNormalizedValueToPlainValue(norm_v);
    float weight = u.params.get(ReverbTankUtilities::kAPFDelayWeight)
                       .convertNormalizedValueToPlainValue(norm_v);
    ASSERT_THAT(
        u.params.get(ReverbTankUtilities::kMaxAPFDelay).getNormalizedValue(),
        Eq(norm_v));
    ASSERT_THAT(
        u.params.get(ReverbTankUtilities::kAPFDelayWeight).getNormalizedValue(),
        Eq(norm_v));

    NestedDelayAPF::NestedDelayAPFParameters expected;
    int m = 0;
    for (int i = 0; i < u.branch_nested_apfs.size(); ++i) {
        expected.delay_ms = max_apf_delay * weight * u.apf_delay_weight[m++];
        expected.inner_apf_delay_ms =
            max_apf_delay * weight * u.apf_delay_weight[m++];
        expected.enable_LFO = true;
        expected.lfo_depth = 1.0f;
        expected.lfo_max_modulation_ms = 0.3;
        expected.apf_g = 0.5;
        expected.inner_apf_g = -0.5;
        switch (i) {
        case 0:
            expected.lfo_rate_hz = 0.15;
            break;
        case 1:
            expected.lfo_rate_hz = 0.33;
            break;
        case 2:
            expected.lfo_rate_hz = 0.57;
            break;
        case 3:
            expected.lfo_rate_hz = 0.73;
            break;
        }

        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().delay_ms,
                    Eq(expected.delay_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().inner_apf_delay_ms,
                    Eq(expected.inner_apf_delay_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().enable_LFO,
                    Eq(expected.enable_LFO));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().lfo_depth,
                    Eq(expected.lfo_depth));
        ASSERT_THAT(
            u.branch_nested_apfs[i].getParameters().lfo_max_modulation_ms,
            Eq(expected.lfo_max_modulation_ms));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().lfo_rate_hz,
                    Eq(expected.lfo_rate_hz));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().apf_g,
                    Eq(expected.apf_g));
        ASSERT_THAT(u.branch_nested_apfs[i].getParameters().inner_apf_g,
                    Eq(expected.inner_apf_g));
    }
}

TEST_F(AReverbTankProcessorUtilities, ProcessUpdateBranchLPFsParameters) {
    float norm_v = 0.5;
    AudioBlock block{{{1, 0, 0, 0}, {0, 0, 0, 0}}, num_params};
    block.param_changes.push(ReverbTankUtilities::kDamping, {0, 0, norm_v});

    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    float expected = u.params.get(ReverbTankUtilities::kDamping)
                         .convertNormalizedValueToPlainValue(norm_v);
    for (int i = 0; i < u.branch_lpfs.size(); ++i) {
        ASSERT_THAT(u.branch_lpfs[i].getParameters().lpf_g, Eq(expected));
    }
}

TEST_F(AReverbTankProcessorUtilities, ProcessUpdateBranchDelaysParameters) {
    float norm_v = 0.5;
    AudioBlock block{{{1, 0, 0, 0}, {0, 0, 0, 0}}, num_params};
    block.param_changes.push(ReverbTankUtilities::kMaxFixedDelay,
                             {0, 0, norm_v});
    block.param_changes.push(ReverbTankUtilities::kFixedDelayWeight,
                             {0, 0, norm_v});

    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    float max_fix_delay = u.params.get(ReverbTankUtilities::kMaxFixedDelay)
                              .convertNormalizedValueToPlainValue(norm_v);
    float weight = u.params.get(ReverbTankUtilities::kFixedDelayWeight)
                       .convertNormalizedValueToPlainValue(norm_v);
    for (int i = 0; i < u.branch_delays.size(); ++i) {
        auto expected = max_fix_delay * weight * u.fixed_delay_weight[i];
        ASSERT_THAT(u.branch_delays[i].getParameters().delay_ms, Eq(expected));
    }
}

TEST_F(AReverbTankProcessorUtilities, ProcessUpdateShelvingFiltersParameters) {
    float norm_v = 0.5;
    AudioBlock block{{{1, 0, 0, 0}, {0, 0, 0, 0}}, num_params};
    block.param_changes.push(ReverbTankUtilities::kLowShelfFc, {0, 0, norm_v});
    block.param_changes.push(ReverbTankUtilities::kLowShelfGainDB,
                             {0, 0, norm_v});
    block.param_changes.push(ReverbTankUtilities::kHighShelfFc, {0, 0, norm_v});
    block.param_changes.push(ReverbTankUtilities::kHighShelfGainDB,
                             {0, 0, norm_v});

    auto low_fc = u.params.get(ReverbTankUtilities::kLowShelfFc)
                      .convertNormalizedValueToPlainValue(norm_v);
    auto low_db = u.params.get(ReverbTankUtilities::kLowShelfGainDB)
                      .convertNormalizedValueToPlainValue(norm_v);
    auto high_fc = u.params.get(ReverbTankUtilities::kHighShelfFc)
                       .convertNormalizedValueToPlainValue(norm_v);
    auto high_db = u.params.get(ReverbTankUtilities::kHighShelfGainDB)
                       .convertNormalizedValueToPlainValue(norm_v);

    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    for (int i = 0; i < u.shelving_filters.size(); ++i) {
        ASSERT_THAT(u.shelving_filters[i].getParameters().low_shelf_fc,
                    Eq(low_fc));
        ASSERT_THAT(u.shelving_filters[i].getParameters().low_shelf_boost_db,
                    Eq(low_db));
        ASSERT_THAT(u.shelving_filters[i].getParameters().high_shelf_fc,
                    Eq(high_fc));
        ASSERT_THAT(u.shelving_filters[i].getParameters().high_shelf_boost_db,
                    Eq(high_db));
    }
}

TEST_F(AReverbTankProcessorUtilities, CanProcess) {
    AudioBlock block{{{1, 0, 0, 0}, {1, 0, 0, 0}}, num_params};

    sample_rate = 2;
    u.prepareToPlay(sample_rate, max_block_size);
    u.process(&block);

    ASSERT_THAT(block.buffer.getWriterPointer(0)[0], FloatNear(0.998401, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(0)[1],
                FloatNear(-0.00174133, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(0)[2],
                FloatNear(-0.00169252, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(0)[3],
                FloatNear(-0.00170218, 1e-5));

    ASSERT_THAT(block.buffer.getWriterPointer(1)[0], FloatNear(1.0, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(1)[1], FloatNear(0.0f, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(1)[2], FloatNear(0.0f, 1e-5));
    ASSERT_THAT(block.buffer.getWriterPointer(1)[3], FloatNear(0.0f, 1e-5));
}
