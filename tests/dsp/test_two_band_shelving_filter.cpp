//
// Created by user on 4/18/22.
//
#include "libaa/dsp/aa_two_band_shelving_filter.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;
class ATwoBandShelvingFilter : public Test {
public:
    TwoBandShelvingFilter f;
    const float sample_rate = 44100;
    TwoBandShelvingFilter::TwoBandShelvingFilterParameters parameters;
};

TEST_F(ATwoBandShelvingFilter, NeedPrepareBeforeUse) {
    f.prepare(sample_rate);
}

TEST_F(ATwoBandShelvingFilter, CanUpdateParameters) {
    f.prepare(sample_rate);

    parameters.low_shelf_fc = 1;
    parameters.low_shelf_boost_db = 1;
    parameters.high_shelf_fc = 1;
    parameters.high_shelf_boost_db = 1;
    f.updateParameters(parameters);

    ASSERT_THAT(f.getParameters().low_shelf_fc, Eq(parameters.low_shelf_fc));
    ASSERT_THAT(f.getParameters().low_shelf_boost_db,
                Eq(parameters.low_shelf_boost_db));
    ASSERT_THAT(f.getParameters().high_shelf_fc, Eq(parameters.high_shelf_fc));
    ASSERT_THAT(f.getParameters().high_shelf_boost_db,
                Eq(parameters.high_shelf_boost_db));
}

TEST_F(ATwoBandShelvingFilter,
       UpdateParametersAlsoUpdateLowShelvingFilterParameters) {
    f.prepare(sample_rate);

    parameters.low_shelf_fc = 100;
    parameters.low_shelf_boost_db = 1;
    f.updateParameters(parameters);

    ASSERT_THAT(f.getLowShelvingFilter().getParameters().type,
                Eq(FilterType::kLowShelf));
    ASSERT_THAT(f.getLowShelvingFilter().getParameters().fc,
                Eq(parameters.low_shelf_fc));
    ASSERT_THAT(f.getLowShelvingFilter().getParameters().boost_or_cut_db,
                Eq(parameters.low_shelf_boost_db));
}

TEST_F(ATwoBandShelvingFilter,
       UpdateParametersAlsoUpdateHighShelvingFilterParameters) {
    f.prepare(sample_rate);

    parameters.high_shelf_fc = 1000;
    parameters.high_shelf_boost_db = 5;
    f.updateParameters(parameters);

    ASSERT_THAT(f.getHighShelvingFilter().getParameters().type,
                Eq(FilterType::kHighShelf));
    ASSERT_THAT(f.getHighShelvingFilter().getParameters().fc,
                Eq(parameters.high_shelf_fc));
    ASSERT_THAT(f.getHighShelvingFilter().getParameters().boost_or_cut_db,
                Eq(parameters.high_shelf_boost_db));
}

TEST_F(ATwoBandShelvingFilter, ProcessGetFilterdSample) {
    f.prepare(sample_rate);

    parameters.low_shelf_fc = 100;
    parameters.low_shelf_boost_db = 1;
    parameters.high_shelf_fc = 2000;
    parameters.high_shelf_boost_db = 1;
    f.updateParameters(parameters);

    ASSERT_THAT(f.processSample(1.0f), FloatNear(0.0104303f, 1e-6));
    ASSERT_THAT(f.processSample(1.0f), FloatNear(0.0265744, 1e-6));
}