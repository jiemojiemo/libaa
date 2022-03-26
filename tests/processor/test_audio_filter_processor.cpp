//
// Created by hw on 2021/12/8.
//

#include "aa_testing_helper.h"
#include "libaa/audio_effect/aa_audio_filter_processor.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class AAudioFilterProcessor : public Test {
public:
    AudioBlock generateExpectedOutput(const AudioBlock &in_block,
                                      const IIRFilterParameter &iir_params) {
        AudioBlock expected_output{in_block};
        AudioFilterUtilities utilities;
        utilities.updateCoeffes(iir_params, sample_rate);

        for (auto c = 0u; c < expected_output.audio_buffer.getNumberChannels();
             ++c) {
            auto &b = utilities.biquad_array[c];
            float *in_out = expected_output.audio_buffer.getWriterPointer(c);
            for (auto i = 0u;
                 i < expected_output.audio_buffer.getNumberFrames(); ++i) {
                in_out[i] = b.processSample(in_out[i]);
            }
        }

        return expected_output;
    }

    AudioFilterProcessor p{};
    float sample_rate = 44100;
    int max_block_size = 100;
};

TEST_F(AAudioFilterProcessor, CanConstruct) { AudioFilterProcessor p{}; }

TEST_F(AAudioFilterProcessor, CanReportCorrectName) {
    ASSERT_THAT(p.getName(), Eq("Filter"));
}

TEST_F(AAudioFilterProcessor, FilterTypeParameterAsExpected) {
    auto param = p.getParameters().get(0);

    ASSERT_THAT(param.getParameterName(), Eq("Filter Type"));
    ASSERT_THAT(param.getDefaultPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(),
                Eq(float(FilterType::kNumFilterType)));
}

TEST_F(AAudioFilterProcessor, FCParameterAsExpected) {
    auto param = p.getParameters().get(1);

    ASSERT_THAT(param.getParameterName(), Eq("FC"));
    ASSERT_THAT(param.getDefaultPlainValue(), Eq(20));
    ASSERT_THAT(param.getMinPlainValue(), Eq(20));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20500));
}

TEST_F(AAudioFilterProcessor, QParameterAsExpected) {
    auto param = p.getParameters().get(2);

    ASSERT_THAT(param.getParameterName(), Eq("Q"));
    ASSERT_THAT(param.getDefaultPlainValue(), FloatEq(0.707f));
    ASSERT_THAT(param.getMinPlainValue(), Eq(0));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(20));
}

TEST_F(AAudioFilterProcessor, GainParameterAsExpected) {
    auto param = p.getParameters().get(3);

    ASSERT_THAT(param.getParameterName(), Eq("Gain"));
    ASSERT_THAT(param.getDefaultPlainValue(), FloatEq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-35));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35));
}

TEST_F(AAudioFilterProcessor, HasZeroLantency) {
    ASSERT_THAT(p.getLatencySamples(), Eq(0));
}

TEST_F(AAudioFilterProcessor, HasZeroTail) {
    ASSERT_THAT(p.getTailLengthSamples(), Eq(0));
}

TEST_F(AAudioFilterProcessor, PrepareWillUpdateCoeffs) {
    p.prepareToPlay({sample_rate, max_block_size});
}

TEST_F(AAudioFilterProcessor, CanProcessSamples) {
    AudioBlock in_block{{{1.0, 1.1, 1.2}, {2.0, 2.1, 2.2}}};

    IIRFilterParameter iir_params;
    AudioBlock expected_output = generateExpectedOutput(in_block, iir_params);

    p.prepareToPlay({sample_rate, max_block_size});
    p.processBlock(&in_block, &in_block);

    ASSERT_THAT(in_block.audio_buffer, Eq(expected_output.audio_buffer));
}

TEST_F(AAudioFilterProcessor,
       ParameterBeChangedIfThereComesParameterChangesEvent) {
    AudioBlock in_block{{{1.0, 1.1, 1.2}, {2.0, 2.1, 2.2}}};
    float norm_value = 0.5;
    in_block.param_changes = ParameterChanges{{0, 0, norm_value},
                                              {1, 0, norm_value},
                                              {2, 0, norm_value},
                                              {3, 0, norm_value}};

    p.prepareToPlay({sample_rate, max_block_size});
    p.processBlock(&in_block, &in_block);

    ASSERT_THAT(p.getParameters().get(0).getNormalizedValue(), Eq(0.5));
    ASSERT_THAT(p.getParameters().get(1).getNormalizedValue(), Eq(0.5));
    ASSERT_THAT(p.getParameters().get(2).getNormalizedValue(), Eq(0.5));
    ASSERT_THAT(p.getParameters().get(3).getNormalizedValue(), Eq(0.5));
}

TEST_F(AAudioFilterProcessor, ProcessAsExpectedAfterParametChanges) {
    AudioBlock in_block{{{1.0, 1.1, 1.2}, {2.0, 2.1, 2.2}}};

    IIRFilterParameter iir_params;
    iir_params.type = libaa::FilterType::kBPF2;
    iir_params.fc = 500;
    iir_params.Q = 1;
    iir_params.boost_or_cut_db = 2;

    AudioBlock expected_output = generateExpectedOutput(in_block, iir_params);

    float type_norm_value =
        p.getParameters().get(0).convertPlainValueToNormalizedValue(
            float(iir_params.type));
    float fc_norm_value =
        p.getParameters().get(1).convertPlainValueToNormalizedValue(
            iir_params.fc);
    float Q_norm_value =
        p.getParameters().get(2).convertPlainValueToNormalizedValue(
            iir_params.Q);
    float gain_norm_value =
        p.getParameters().get(3).convertPlainValueToNormalizedValue(
            iir_params.boost_or_cut_db);

    in_block.param_changes = ParameterChanges{{0, 0, type_norm_value},
                                              {1, 0, fc_norm_value},
                                              {2, 0, Q_norm_value},
                                              {3, 0, gain_norm_value}};
    p.prepareToPlay({sample_rate, max_block_size});
    p.processBlock(&in_block, &in_block);

    ASSERT_THAT(in_block.audio_buffer, Eq(expected_output.audio_buffer));
}