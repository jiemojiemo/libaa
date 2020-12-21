
//
// Created by William.Hua on 2020/12/20.
//

#include "audio_effect/aa_biquad_impl.h"
#include <array>
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

enum class FilterType
{
    kLPF1 = 0,          // 1st order low-pass filter
    kHPF1,              // 1st order high-pass filter
    kLPF2,              // 2nd order low-pass filter
    kHPF2,              // 2nd order high-pass filter
    kBPF2,              // 2nd order band-pass filter
    kBSF2,              // 2nd order band-stop filter
    kButterLPF2,        // 2nd order Butterworth low-pass filter
    kButterHPF2,        // 2nd order Butterworth high-pass filter
    kButterBPF2,        // 2nd order Butterworth band-pass filter
    kButterBSF2,        // 2nd order Butterworth band-stop filter
    kLWRLPF2,           // 2nd order Linkwitz-Riley low-pass filter
    kLWRHPF2,           // 2nd order Linkwitz-Riley high-pass filter
    kAPF1,              // 1st order APF filter
    kAPF2,              // 2nd order APF filter
    kLowShelf,          // 1sr order low shelving filter
    kHighShelf,         // 1sr order high shelving filter
    kNCQParaEQ,         // 2nd order non-constant Q parametric EQ filter
    kCQParaEQ,          // 2nd order constant Q parametric EQ filter
};

class IIRFilterParameter
{
public:
    FilterType type         {FilterType::kLPF1};
    float sample_rate       {0.0f};
    float fc                {0.0f};
    float Q                 {0.0f};
    float boost_or_cut_db   {0.0f};
};

class IIRFilter
{
public:
    static FilterCoeffs calcFilterCoeffs(const IIRFilterParameter& iir_param)
    {
        FilterCoeffs coeffs{0.0f, };

        coeffs[Biquad::c0] = 1.0f;
        coeffs[Biquad::d0] = 0.0f;

        const auto type = iir_param.type;
        const auto fc = iir_param.fc;
        const auto sample_rate = iir_param.sample_rate;
        const auto Q = iir_param.Q;
        const auto boost_cut_db = iir_param.boost_or_cut_db;

        if(type == FilterType::kLPF1){
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto gamma = cos(theta_c) / (1 + sin(theta_c));

            coeffs[Biquad::a0] = (1 - gamma) / 2;
            coeffs[Biquad::a1] = (1 - gamma) / 2;
            coeffs[Biquad::a2] = 0.0f;
            coeffs[Biquad::b1] = -gamma;
            coeffs[Biquad::b2] = 0.0f;

        }else if(type == FilterType::kHPF1){
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto gamma = cos(theta_c) / (1 + sin(theta_c));

            coeffs[Biquad::a0] = (1 + gamma) / 2;
            coeffs[Biquad::a1] = -(1 + gamma) / 2;
            coeffs[Biquad::a2] = 0.0f;
            coeffs[Biquad::b1] = -gamma;
            coeffs[Biquad::b2] = 0.0f;

        }else if(type == FilterType::kLPF2){
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto d = 1 / Q;
            const auto beta = 0.5 * (1 - d/2*sin(theta_c))/(1 + d/2*sin(theta_c));
            const auto gama = (0.5f + beta) * cos(theta_c);

            coeffs[Biquad::a0] = (0.5 + beta - gama) / 2.0f;
            coeffs[Biquad::a1] = (0.5 + beta - gama);
            coeffs[Biquad::a2] = (0.5 + beta - gama) / 2.0f;
            coeffs[Biquad::b1] = -2 * gama;
            coeffs[Biquad::b2] = 2 * beta;

        }else if(type == FilterType::kHPF2){
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto d = 1 / Q;
            const auto beta = 0.5 * (1 - d/2*sin(theta_c))/(1 + d/2*sin(theta_c));
            const auto gama = (0.5f + beta) * cos(theta_c);

            coeffs[Biquad::a0] = (0.5 + beta + gama) / 2.0f;
            coeffs[Biquad::a1] = -(0.5 + beta + gama);
            coeffs[Biquad::a2] = (0.5 + beta + gama) / 2.0f;
            coeffs[Biquad::b1] = -2 * gama;
            coeffs[Biquad::b2] = 2 * beta;

        }else if(type == FilterType::kBPF2){
            const auto K = tan(M_PI * fc / sample_rate);
            const auto delta = K*K*Q + K + Q;

            coeffs[Biquad::a0] = K/delta;
            coeffs[Biquad::a1] = 0.0f;
            coeffs[Biquad::a2] = -K/delta;
            coeffs[Biquad::b1] = 2*Q*(K*K - 1) / delta;
            coeffs[Biquad::b2] = (K*K*Q - K + Q) /  delta;

        }else if(type == FilterType::kBSF2){
            const auto K = tan(M_PI * fc / sample_rate);
            const auto delta = K*K*Q + K + Q;

            coeffs[Biquad::a0] = Q*(K*K + 1) / delta;
            coeffs[Biquad::a1] = 2*Q*(K*K - 1) / delta;
            coeffs[Biquad::a2] = Q*(K*K + 1) / delta;
            coeffs[Biquad::b1] = 2*Q*(K*K - 1) / delta;
            coeffs[Biquad::b2] = (K*K*Q - K + Q) /  delta;

        }else if(type == FilterType::kButterLPF2){
            const auto C = 1.0f / (tan(M_PI * fc / sample_rate));

            coeffs[Biquad::a0] = 1.0f / (1 + M_SQRT2*C + C*C);
            coeffs[Biquad::a1] = 2*coeffs[Biquad::a0];
            coeffs[Biquad::a2] = coeffs[Biquad::a0];
            coeffs[Biquad::b1] = coeffs[Biquad::a1] * (1 - C*C);
            coeffs[Biquad::b2] = coeffs[Biquad::a0] * (1 - M_SQRT2*C + C*C);

        }else if(type == FilterType::kButterHPF2){
            const auto C = 1.0f / (tan(M_PI * fc / sample_rate));

            coeffs[Biquad::a0] = 1.0f / (1 + M_SQRT2*C + C*C);
            coeffs[Biquad::a1] = -2*coeffs[Biquad::a0];
            coeffs[Biquad::a2] = coeffs[Biquad::a0];
            coeffs[Biquad::b1] = coeffs[Biquad::a1] * (C*C - 1);
            coeffs[Biquad::b2] = coeffs[Biquad::a0] * (1 - M_SQRT2*C + C*C);

        }else if(type == FilterType::kButterBPF2){
            const auto BW = fc / Q;
            const auto C = 1.0f / (tan(M_PI * fc * BW / sample_rate));
            const auto D = 2.0f * cos(2 * M_PI * fc / sample_rate);

            coeffs[Biquad::a0] = 1.0f / (1 + C) ;
            coeffs[Biquad::a1] = 0.0f;
            coeffs[Biquad::a2] = -coeffs[Biquad::a0];
            coeffs[Biquad::b1] = -coeffs[Biquad::a0] * C * D;
            coeffs[Biquad::b2] = coeffs[Biquad::a0] * (C - 1);

        }else if(type == FilterType::kButterBSF2){
            const auto BW = fc / Q;
            const auto C = 1.0f / (tan(M_PI * fc * BW / sample_rate));
            const auto D = 2.0f * cos(2 * M_PI * fc / sample_rate);

            coeffs[Biquad::a0] = 1.0f / (1 + C) ;
            coeffs[Biquad::a1] = -coeffs[Biquad::a0] * D;
            coeffs[Biquad::a2] = coeffs[Biquad::a0];
            coeffs[Biquad::b1] = -coeffs[Biquad::a0] * D;
            coeffs[Biquad::b2] = coeffs[Biquad::a0] * (1 - C);

        }else if(type == FilterType::kLWRLPF2){
            const auto theta_c = M_PI * fc / sample_rate;
            const auto omega_c = M_PI * fc;
            const auto k = omega_c / tan(theta_c);
            const auto delta = k*k + omega_c*omega_c + 2*k*omega_c;

            const auto b1_num = -2 * k*k + 2 * omega_c*omega_c;
            const auto b2_num = -2 * k*omega_c + k*k + omega_c*omega_c;

            coeffs[Biquad::a0] = omega_c*omega_c / delta;
            coeffs[Biquad::a1] = 2 * coeffs[Biquad::a0];
            coeffs[Biquad::a2] = coeffs[Biquad::a0];
            coeffs[Biquad::b1] = b1_num / delta;
            coeffs[Biquad::b2] = b2_num / delta;

        }else if(type == FilterType::kLWRHPF2){
            const auto theta_c = M_PI * fc / sample_rate;
            const auto omega_c = M_PI * fc;
            const auto k = omega_c / tan(theta_c);
            const auto delta = k*k + omega_c*omega_c + 2*k*omega_c;

            const auto b1_num = -2 * k*k + 2 * omega_c*omega_c;
            const auto b2_num = -2 * k*omega_c + k*k + omega_c*omega_c;

            coeffs[Biquad::a0] = k*k / delta;
            coeffs[Biquad::a1] = -2 * coeffs[Biquad::a0];
            coeffs[Biquad::a2] = coeffs[Biquad::a0];
            coeffs[Biquad::b1] = b1_num / delta;
            coeffs[Biquad::b2] = b2_num / delta;

        }else if(type == FilterType::kAPF1){
            const auto alpha_num = tan(M_PI * fc / sample_rate) - 1;
            const auto alpha_dec = tan(M_PI * fc / sample_rate) + 1;
            const auto alpha = alpha_num / alpha_dec;


            coeffs[Biquad::a0] = alpha;
            coeffs[Biquad::a1] = 1.0f;
            coeffs[Biquad::a2] = 0.0f;
            coeffs[Biquad::b1] = alpha;
            coeffs[Biquad::b2] = 0.0f;
        }else if(type == FilterType::kAPF2){
            const auto BW = fc / Q;
            const auto alpha_num = tan(BW*M_PI / sample_rate) - 1;
            const auto alpha_den = tan(BW*M_PI / sample_rate) + 1;
            const auto alpha = alpha_num / alpha_den;
            const auto beta = -cos(2*M_PI*fc / sample_rate);


            coeffs[Biquad::a0] = -alpha;
            coeffs[Biquad::a1] = beta * (1 - alpha);
            coeffs[Biquad::a2] = 1.0;
            coeffs[Biquad::b1] = coeffs[Biquad::a1];
            coeffs[Biquad::b2] = -alpha;
        }else if(type == FilterType::kLowShelf){
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);
            const auto beta = 4.0 / (1 + mu);
            const auto delta = beta * tan(theta_c / 2);
            const auto gamma = (1 - delta) / (1 + delta);


            coeffs[Biquad::a0] = (1 - gamma) / 2;
            coeffs[Biquad::a1] = coeffs[Biquad::a0];
            coeffs[Biquad::a2] = 0.0f;
            coeffs[Biquad::b1] = -gamma;
            coeffs[Biquad::b2] = 0.0f;
            coeffs[Biquad::c0] = mu - 1.0f;
            coeffs[Biquad::d0] = 1.0f;
        }else if(type == FilterType::kHighShelf){
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);
            const auto beta = 4.0 / (1 + mu);
            const auto delta = beta * tan(theta_c / 2);
            const auto gamma = (1 - delta) / (1 + delta);


            coeffs[Biquad::a0] = (1 + gamma) / 2;
            coeffs[Biquad::a1] = -coeffs[Biquad::a0];
            coeffs[Biquad::a2] = 0.0f;
            coeffs[Biquad::b1] = -gamma;
            coeffs[Biquad::b2] = 0.0f;
            coeffs[Biquad::c0] = mu - 1.0f;
            coeffs[Biquad::d0] = 1.0f;
        }else if(type == FilterType::kNCQParaEQ){
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);

            auto tan_arg = theta_c / (2.0 * Q);
            if(tan_arg >= 0.95*M_PI_2){
                tan_arg = 0.95*M_PI_2;
            }
            const auto zeta = 4.0 / (1.0 + mu);
            const auto beta_num = 1.0 - zeta * tan(tan_arg);
            const auto beta_den = 1.0 + zeta * tan(tan_arg);
            const auto beta = 0.5 * beta_num/beta_den;
            const auto gamma = (0.5 + beta) * cos(theta_c);


            coeffs[Biquad::a0] = 0.5 - beta;
            coeffs[Biquad::a1] = 0.0f;
            coeffs[Biquad::a2] = -coeffs[Biquad::a0];
            coeffs[Biquad::b1] = -2 * gamma;
            coeffs[Biquad::b2] = 2 * beta;
            coeffs[Biquad::c0] = mu - 1.0f;
            coeffs[Biquad::d0] = 1.0f;
        }else if(type == FilterType::kCQParaEQ){
            // --- see book for formulae
            const auto K = tan(M_PI*fc / sample_rate);
            const auto Vo = pow(10.0, boost_cut_db / 20.0);
            const bool bBoost = boost_cut_db >= 0;

            const auto d0 = 1.0 + (1.0 / Q)*K + K*K;
            const auto e0 = 1.0 + (1.0 / (Vo*Q))*K + K*K;
            const auto alpha = 1.0 + (Vo / Q)*K + K*K;
            const auto beta = 2.0*(K*K - 1.0);
            const auto gamma = 1.0 - (Vo / Q)*K + K*K;
            const auto delta = 1.0 - (1.0 / Q)*K + K*K;
            const auto eta = 1.0 - (1.0 / (Vo*Q))*K + K*K;

             coeffs[Biquad::a0] = bBoost ? alpha / d0 : d0 / e0;
             coeffs[Biquad::a1] = bBoost ? beta / d0 : beta / e0;
             coeffs[Biquad::a2] = bBoost ? gamma / d0 : delta / e0;
             coeffs[Biquad::b1] = bBoost ? beta / d0 : beta / e0;
             coeffs[Biquad::b2] = bBoost ? delta / d0 : eta / e0;
        }
        return coeffs;
    }
};

MATCHER_P(FloatNearPointwise, tol, "Out of range") {
    return (std::get<0>(arg)>std::get<1>(arg)-tol && std::get<0>(arg)<std::get<1>(arg)+tol) ;
}

class AIIRFilter : public Test
{
public:
    IIRFilter filter;
    IIRFilterParameter iir_params;
};

TEST_F(AIIRFilter, CanCalc1stOrderLPFCoefficents)
{
    iir_params = {FilterType::kLPF1,
                  2000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{1, 1, 0.0, 1.0, 0.0, 1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc1stOrderHPFCoefficents)
{
    iir_params = {FilterType::kHPF1,
                  2000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0, 0, 0.0, 1.0, 0.0, 1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderLPFCoefficents)
{
    iir_params = {FilterType::kLPF2,
                  2000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{1.0, 2, 1, 2, 1,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderHPFCoefficents)
{
    iir_params = {FilterType::kHPF2,
                  2000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0, 0, 0, 2, 1,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderBPFCoefficents)
{
    iir_params = {FilterType::kBPF2,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{1.0f/3, 0, -1.0f/3, 0, 1.0f/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderBSFCoefficents)
{
    iir_params = {FilterType::kBSF2,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{2.0f/3, 0, 2.0f/3, 0, 1.0f/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-12), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthLPFCoefficents)
{
    iir_params = {FilterType::kButterLPF2,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.2928, 0.5857, 0.2928, 0, 0.1715,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthHPFCoefficents)
{
    iir_params = {FilterType::kButterHPF2,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.2928, -0.5857, 0.2928, 0, 0.1715,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthBPFCoefficents)
{
    iir_params = {FilterType::kButterBPF2,
                  800.0f,
                  20.0f,
                  2.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.5, 0.0, -0.5, -0.9876883405951378, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderButterworthBSFCoefficents)
{
    iir_params = {FilterType::kButterBSF2,
                  800.0f,
                  20.0f,
                  2.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.5, -0.9876883405951378, 0.5, -0.9876883405951378, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcLWRLPF2Coefficents)
{
    iir_params = {FilterType::kLWRLPF2,
                  4000.0f,
                  1000.0f,
                  2.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.25, 0.5, 0.25, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalckLWRHPF2Coefficents)
{
    iir_params = {FilterType::kLWRHPF2,
                  4000.0f,
                  1000.0f,
                  2.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.25, -0.5, 0.25, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc1srOrderAPFCoefficents)
{
    iir_params = {FilterType::kAPF1,
                  4000.0f,
                  1000.0f,
                  2.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.0, 1.0, 0.0, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderkAPFCoefficents)
{
    iir_params = {FilterType::kAPF2,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  10.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.0, 0.0, 1.0, 0, 0.0,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcLowShelfvingCoefficents)
{
    iir_params = {FilterType::kLowShelf,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  0.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{2.0/3, 2.0/3, 0.0, 1.0/3, 0.0,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalcHighShelfvingCoefficents)
{
    iir_params = {FilterType::kHighShelf,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  0.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{1.0/3, -1.0/3, 0.0, 1.0/3, 0.0,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderNonConstantQCoefficents)
{
    iir_params = {FilterType::kNCQParaEQ,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  0.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{2.0/3, 0.0, -2.0/3, 0.0, -1.0/3,0.0, 1.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderConstantQWithBoostCoefficents)
{
    iir_params = {FilterType::kCQParaEQ,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  20.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{4, 0.0, -8.0/3, 0.0, 1.0/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}

TEST_F(AIIRFilter, CanCalc2ndOrderConstantQWithCutCoefficents)
{
    iir_params = {FilterType::kCQParaEQ,
                  4000.0f,
                  1000.0f,
                  1.0f,
                  -20.0f};

    auto result = filter.calcFilterCoeffs(iir_params);

    auto expected_result = FilterCoeffs{0.25, 0.0, 1.0/12, 0.0, -2.0/3,1.0, 0.0};
    ASSERT_THAT(result, Pointwise(FloatNearPointwise(1e-3), expected_result));
}