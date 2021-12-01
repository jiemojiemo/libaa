//
// Created by bytedance on 2021/11/30.
//

#include <gmock/gmock.h>
#include "libaa/dsp/aa_biquad_impl.h"
#include "libaa/dsp/aa_audio_filter_utilities.h"
using namespace testing;
using namespace libaa;

class AAudioFilterUtilities : public Test
{
public:
    float sample_rate = 44100;
    IIRFilterParameter p;
};

TEST_F(AAudioFilterUtilities, Use20AsCutOffFrequencyIfInputFCLessThan20)
{
    p.type = FilterType::kLPF1;
    p.fc = 0;
    auto coeffs = AudioFilterUtilities::calcFilterCoefficients(p, sample_rate);

    IIRFilterParameter expected_p = p;
    expected_p.fc = 20;
    auto expected_coeffs = AudioFilterUtilities::calcFilterCoefficients(expected_p, sample_rate);

    ASSERT_THAT(coeffs, Eq(expected_coeffs));
}

TEST_F(AAudioFilterUtilities, Use93PercentsNyquistAsCutOffFrequencyIfInputFCLargeThanNyquist)
{
    p.type = FilterType::kLPF1;
    p.fc = sample_rate / 2;
    auto coeffs = AudioFilterUtilities::calcFilterCoefficients(p, sample_rate);

    IIRFilterParameter expected_p = p;
    expected_p.fc = sample_rate / 2 * 0.93f;
    auto expected_coeffs = AudioFilterUtilities::calcFilterCoefficients(expected_p, sample_rate);

    ASSERT_THAT(coeffs, Eq(expected_coeffs));
}