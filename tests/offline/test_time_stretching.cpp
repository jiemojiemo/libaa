//
// Created by agen on 4/7/22.
//

#include "RubberBandStretcher.h"
#include "libaa/offline/aa_time_stretching.h"
#include <gmock/gmock.h>
#include <vector>

using namespace std;
using namespace testing;
using namespace libaa;

class ATimeStretching : public Test {
public:
    void SetUp() override {
        left_data.resize(max_block_size, 0.1);
        right_data.resize(max_block_size, 0.2);
        data_refer_to.resize(2);
        data_refer_to[0] = left_data.data();
        data_refer_to[1] = right_data.data();
    }
    TimeStretching ts;
    const int sample_rate = 44100;
    const int max_block_size = 1024;
    const int num_channels = 2;

    vector<float> left_data;
    vector<float> right_data;
    vector<float *> data_refer_to;
};

TEST_F(ATimeStretching, CanReportCorrectName) {
    ASSERT_THAT(ts.getName(), Eq("TimeStretching"));
}

TEST_F(ATimeStretching, CanPrepare) {
    ts.prepare(sample_rate, num_channels, max_block_size);
}

TEST_F(ATimeStretching, HasNullImplWhenInit) {
    ASSERT_THAT(ts.getTsImpl(), IsNull());
}

TEST_F(ATimeStretching, HasNotNullImplAfterPrepare) {
    ts.prepare(sample_rate, num_channels, max_block_size);

    ASSERT_THAT(ts.getTsImpl(), NotNull());
}

TEST_F(ATimeStretching, HasParameters) {
    ASSERT_THAT(ts.getParameters(), NotNull());
}

TEST_F(ATimeStretching, HasTimeRatioParameterAsExpect) {
    auto p = ts.getParameters()->get(0);

    ASSERT_THAT(p.getParameterName(), Eq("Time Ratio"));
    ASSERT_THAT(p.getPlainValue(), FloatEq(1.0f));
    ASSERT_THAT(p.getMinPlainValue(), FloatEq(0.1f));
    ASSERT_THAT(p.getMaxPlainValue(), FloatEq(10.0f));
}

TEST_F(ATimeStretching, HasPitchScaleParameterAsExpect) {
    auto p = ts.getParameters()->get(1);

    ASSERT_THAT(p.getParameterName(), Eq("Pitch Scale"));
    ASSERT_THAT(p.getPlainValue(), FloatEq(1.0f));
    ASSERT_THAT(p.getMinPlainValue(), FloatEq(0.1f));
    ASSERT_THAT(p.getMaxPlainValue(), FloatEq(8.0f));
}

TEST_F(ATimeStretching, AvailableIsZereBeforeProcess) {
    ASSERT_THAT(ts.available(), Eq(0));
}

TEST_F(ATimeStretching, MayHaveAvailableAfterProcess) {
    ts.prepare(sample_rate, num_channels, max_block_size);

    // process multiple times ensure there has available() size
    for (int i = 0; i < 3; ++i) {
        ts.process(data_refer_to.data(), max_block_size, false);
    }

    ASSERT_THAT(ts.available(), Not(0));
}

TEST_F(ATimeStretching, CanRetriveData) {
    ts.prepare(sample_rate, num_channels, max_block_size);
    for (int i = 0; i < 3; ++i) {
        ts.process(data_refer_to.data(), max_block_size, false);
    }
    int available = ts.available();

    ts.retrieve(data_refer_to.data(), available);
}

TEST_F(ATimeStretching, RetriveAwaysReturn0IfNotPrepared) {
    for (int i = 0; i < 3; ++i) {
        ts.process(data_refer_to.data(), max_block_size, false);
    }
    int available = ts.available();

    ASSERT_THAT(available, Eq(0));
}

TEST_F(ATimeStretching, RetriveReturnsActualNumberFramesGot) {
    ts.prepare(sample_rate, num_channels, max_block_size);
    for (int i = 0; i < 3; ++i) {
        ts.process(data_refer_to.data(), max_block_size, false);
    }
    int available = ts.available();
    int want_retrieve_num = available + 100;
    size_t num_retrieve = ts.retrieve(data_refer_to.data(), available);

    ASSERT_THAT(num_retrieve, Eq(available));
}

TEST_F(ATimeStretching, CanPushParameterChangePoint) {
    ts.prepare(sample_rate, num_channels, max_block_size);
    ts.pushParameterChangePoint({0, 0, 0.5});

    auto p = ts.getParameters()->get(0);
    ASSERT_THAT(p.getNormalizedValue(), Eq(0.5));
}

TEST_F(ATimeStretching, PushParameterChangePointFailedIfOutofRange) {
    int output_of_range_index = 10;
    ts.prepare(sample_rate, num_channels, max_block_size);
    bool is_ok = ts.pushParameterChangePoint({output_of_range_index, 0, 0.5});

    ASSERT_FALSE(is_ok);
}

TEST_F(ATimeStretching, CanUpdateRubberbandTimeRatioAfterProcess) {
    int param_index = 0;
    auto time_ratio_plain_value = 3.0f;
    auto time_ratio_norm_value =
        ts.getParameters()
            ->get(param_index)
            .convertPlainValueToNormalizedValue(time_ratio_plain_value);

    ts.prepare(sample_rate, num_channels, max_block_size);
    auto *rubberband =
        reinterpret_cast<RubberBand::RubberBandStretcher *>(ts.getTsImpl());

    ts.pushParameterChangePoint({param_index, 0, time_ratio_norm_value});
    ts.process(data_refer_to.data(), max_block_size, false);

    ASSERT_THAT(rubberband->getTimeRatio(), DoubleNear(time_ratio_plain_value, 1e-6));
}

TEST_F(ATimeStretching, CanUpdateRubberbandPitchScaleAfterProcess) {
    int param_index = 1;
    auto pitch_scale_plain_value = 3.0f;
    auto pitch_scale_norm_value =
        ts.getParameters()
            ->get(param_index)
            .convertPlainValueToNormalizedValue(pitch_scale_plain_value);
    ts.prepare(sample_rate, num_channels, max_block_size);
    auto *rubberband =
        reinterpret_cast<RubberBand::RubberBandStretcher *>(ts.getTsImpl());

    ts.pushParameterChangePoint({param_index, 0, pitch_scale_norm_value});
    ts.process(data_refer_to.data(), max_block_size, false);

    ASSERT_THAT(rubberband->getPitchScale(), Eq(pitch_scale_plain_value));
}