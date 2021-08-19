
//
// Created by William.Hua on 2021/6/4.
//

#include <gmock/gmock.h>
#include "libaa/audio_effect/aa_gain_processor.h"
#include "libaa/dsp/aa_db_utils.h"
#include <algorithm>
#include "aa_test_helper.h"
using namespace testing;
using namespace libaa;


class AGainProcessor : public Test
{
public:
    void SetUp() override
    {
        in_block = AudioBlock({
            {1.f,1.0f},
            {2.f,2.0f}
        });

        out_block = AudioBlock(in_block.audio_buffer.getNumberChannels(), in_block.audio_buffer.getNumberFrames());
    }
    GainProcessor gain;
    AudioBlock in_block;
    AudioBlock out_block;
};

TEST_F(AGainProcessor, CanConstructWithDefault)
{
    GainProcessor gain;
}

TEST_F(AGainProcessor, CanReportCorretName)
{
    ASSERT_THAT(gain.getName(), Eq("Gain"));
}

TEST_F(AGainProcessor, GainParameterDefaultIsExpected)
{
    const AudioProcessorParameter& param = gain.getParameters().get(0);

    ASSERT_THAT(param.getParameterType(), Eq(ParameterType::kFloat));
    ASSERT_THAT(param.getDefaultPlainValue(), Eq(0));
    ASSERT_THAT(param.getMinPlainValue(), Eq(-75));
    ASSERT_THAT(param.getMaxPlainValue(), Eq(35));
}

TEST_F(AGainProcessor, CanContructWithGainDb)
{
    gain = GainProcessor{2.0};
    const AudioProcessorParameter& param = gain.getParameters().get(0);

    ASSERT_THAT(param.getPlainValue(), Eq(2.0f));
}

TEST_F(AGainProcessor, DataNotChangedIfGainDbIsZero)
{
    gain.process(&in_block, &out_block);

    ASSERT_THAT(in_block.audio_buffer, Eq(out_block.audio_buffer));
}

TEST_F(AGainProcessor, ApplyStaticGainIfThereHasNoParameterChange)
{
    float gain_db = 2.0;

    gain = GainProcessor{gain_db};

    gain.process(&in_block, &out_block);

    AudioBufferNew<float> expected(
        {
            {1.0f * dbToScale(gain_db),  1.0f * dbToScale(gain_db)},
            {2.0f * dbToScale(gain_db),  2.0f * dbToScale(gain_db)},
        }
        );

    ASSERT_THAT(out_block.audio_buffer, Eq(expected));
}

TEST_F(AGainProcessor, ApplyDynamicGainIfThereHasParameterChange)
{
    float start_gain_db = 0.0f;
    float normalized_start_gain = (start_gain_db + 75) / (35 + 75);
    gain = GainProcessor{start_gain_db};

    float normalized_end_gain = 1.0f;
    ParameterChanges changes{
        {0, 0, normalized_end_gain},
        };

    auto in_buffer = AudioBufferNew<float>(
            {
                {1.f,1.0f},
                {2.f,2.0f}
            }
            );

    in_block = AudioBlock(in_buffer, changes);

    gain.process(&in_block, &out_block);

    auto getRampValue = [](size_t current_sample, size_t num_samples, float start_value, float end_value){
        int num_step = std::min(int(num_samples - 1), 1);

        float step = (end_value - start_value)/num_step;
        return start_value + step * current_sample;
    };

    auto expected_out = in_buffer;
    for(auto i = 0u; i < expected_out.getNumberFrames(); ++i)
    {
        float dynamic_normalized_gain = getRampValue(i, expected_out.getNumberFrames(), normalized_start_gain, normalized_end_gain);
        AudioProcessorParameter p = gain.getParameters().get(0);
        p.setNormalizedValue(dynamic_normalized_gain);

        float dynamic_gain_scale = dbToScale(p.getPlainValue());

        for(auto c = 0u; c < expected_out.getNumberChannels(); ++c)
        {
            expected_out.getWriterPointer(c)[i] *= dynamic_gain_scale;
        }
    }

    ASSERT_THAT(out_block.audio_buffer, Eq(expected_out));
}

