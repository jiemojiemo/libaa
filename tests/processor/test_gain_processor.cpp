
//
// Created by William.Hua on 2021/6/4.
//

#include <gmock/gmock.h>
#include "libaa/audio_effect/aa_gain_processor.h"
#include "libaa/dsp/aa_db_utils.h"
#include "aa_test_helper.h"

using namespace testing;
using namespace libaa;


class AGainProcessor : public Test
{
public:
    GainProcessor gain;
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
    AudioBufferNew<float> in_buffer({{1,2,3},
                                     {1,2,3}});
    AudioBufferNew<float> out_buffer(in_buffer.getNumberChannels(), in_buffer.getNumberFrames());

    gain.process(&in_buffer, &out_buffer);

    ASSERT_THAT(in_buffer, Eq(out_buffer));
}

TEST_F(AGainProcessor, ApplyGain)
{
    float gain_db = 2.0;
    AudioBufferNew<float> in_buffer({
                                        {1.f},
                                        {2.f}
                                    });
    AudioBufferNew<float> out_buffer(in_buffer.getNumberChannels(), in_buffer.getNumberFrames());
    gain = GainProcessor{gain_db};

    gain.process(&in_buffer, &out_buffer);

    AudioBufferNew<float> expected(
        {
            {1.0f * db_to_scale(gain_db)},
            {2.0f * db_to_scale(gain_db)},
        }
        );

    ASSERT_THAT(out_buffer, Eq(expected));
}

TEST_F(AGainProcessor, CanSetParameter)
{
    int ret = gain.setParameter(0, 0.5);

    ASSERT_THAT(ret, Eq(0));
}

TEST_F(AGainProcessor, SetParameterFailedIfIndexNotExist)
{
    int ret = gain.setParameter(10, 0.5);

    ASSERT_THAT(ret, Eq(-1));
}

TEST_F(AGainProcessor, ParameterChangedAfterSetParameter)
{
    gain.setParameter(0, 0.5);
    const AudioProcessorParameter& params = gain.getParameters().get(0);

    ASSERT_THAT(params.getNormalizedValue(), 0.5);
}