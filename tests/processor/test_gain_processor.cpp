
//
// Created by William.Hua on 2021/6/4.
//

#include <gmock/gmock.h>
#include "libaa/audio_effect/aa_gain_processor.h"
#include "libaa/dsp/aa_db_utils.h"

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
    auto* param = gain.getParameters().get(0);

    ASSERT_THAT(param, NotNull());
    ASSERT_THAT(param->getParameterType(), Eq(ParameterType::kFloat));
    ASSERT_THAT(param->getDefaultPlainValue(), Eq(0));
    ASSERT_THAT(param->getMinPlainValue(), Eq(-75));
    ASSERT_THAT(param->getMaxPlainValue(), Eq(35));
}

TEST_F(AGainProcessor, CanContructWithGainDb)
{
    gain = GainProcessor{2.0};
    auto* param = gain.getParameters().get(0);

    ASSERT_THAT(param->getPlainValue(), Eq(2.0f));
}

MATCHER_P(FloatNearPointwise, tol, "Out of range") {
    return (std::get<0>(arg)>std::get<1>(arg)-tol && std::get<0>(arg)<std::get<1>(arg)+tol) ;
}

TEST_F(AGainProcessor, DataNotChangedIfGainDbIsZero)
{
    int num_block_size = 10;
    std::vector<float> in_data(num_block_size, 0.5);
    std::vector<float> out_data(num_block_size);

    gain.process(in_data.data(), in_data.size(), out_data.data(), out_data.size());

    ASSERT_THAT(out_data, Pointwise( FloatNearPointwise(1e-8), in_data));
}

TEST_F(AGainProcessor, ApplyGain)
{
    int num_block_size = 10;
    float input_scale = 0.5;
    float gain_db = 2.0;
    std::vector<float> in_data(num_block_size, input_scale);
    std::vector<float> out_data(num_block_size);
    gain = GainProcessor{gain_db};

    gain.process(in_data.data(), in_data.size(), out_data.data(), out_data.size());
    std::vector<float> expected(num_block_size, input_scale*db_to_scale(gain_db));

    ASSERT_THAT(out_data, Pointwise( FloatNearPointwise(1e-8), expected));
}