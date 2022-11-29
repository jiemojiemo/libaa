//
// Created by user on 7/21/22.
//
#include "libaa/mir/aa_loudness_ebur128_extractor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class ALoudnessEBUR128Extractor : public Test {
public:
    LoudnessEBUR128Extractor e;

    float sample_rate = 44100;
    size_t num_channels = 1;
};

TEST_F(ALoudnessEBUR128Extractor, CanReportCorrectName) {
    ASSERT_THAT(e.getName(), Eq("LoudnessEBUR128"));
}

TEST_F(ALoudnessEBUR128Extractor, ConfigureInternalImplementation) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getEBURState(), NotNull());
}

TEST_F(ALoudnessEBUR128Extractor, FrameFeauresHasContainsMomentaryAndShortterm) {
    num_channels = 1;
    sample_rate = 16;
    std::vector<float> r{2048, 0.2};
    std::vector<float *> data_refer_to{r.data()};

    e.initialize(sample_rate, num_channels);
    e.configure();
    e.process(data_refer_to.data(), r.size(), true);

    auto feature = e.getFrameFeatures();
    ASSERT_THAT(feature.size(), Eq(5));
    ASSERT_THAT(feature.at(0)[0].label, Eq("momentary"));
    ASSERT_THAT(feature.at(1)[0].label, Eq("short term loudness"));
    ASSERT_THAT(feature.at(2)[0].label, Eq("integrated loudness"));
    ASSERT_THAT(feature.at(3)[0].label, Eq("loudness range"));
    ASSERT_THAT(feature.at(4)[0].label, Eq("true peak"));
}

TEST_F(ALoudnessEBUR128Extractor, TruePeakSizeSameWithNumberChannels) {
    num_channels = 2;
    sample_rate = 16;
    std::vector<float> r{2048, 0.2};
    std::vector<float *> data_refer_to{r.data(), r.data()};

    e.initialize(sample_rate, num_channels);
    e.configure();
    e.process(data_refer_to.data(), r.size(), true);

    auto feature = e.getFrameFeatures();
    ASSERT_THAT(feature.at(4)[0].label, Eq("true peak"));
    ASSERT_THAT(feature.at(4)[0].values.size(), Eq(num_channels));
}