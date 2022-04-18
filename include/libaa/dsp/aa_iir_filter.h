//
// Created by user on 4/18/22.
//

#ifndef LIBAA_AA_IIR_FILTER_H
#define LIBAA_AA_IIR_FILTER_H

#pragma once
#include "libaa/core/aa_math_tools.h"
#include "libaa/dsp/aa_audio_filter_utilities.h"
#include <cfloat>
#include <cmath>
namespace libaa {
enum class FilterType {
    kLPF1 = 0,   // 1st order low-pass filter
    kHPF1,       // 1st order high-pass filter
    kLPF2,       // 2nd order low-pass filter
    kHPF2,       // 2nd order high-pass filter
    kBPF2,       // 2nd order band-pass filter
    kBSF2,       // 2nd order band-stop filter
    kButterLPF2, // 2nd order Butterworth low-pass filter
    kButterHPF2, // 2nd order Butterworth high-pass filter
    kButterBPF2, // 2nd order Butterworth band-pass filter
    kButterBSF2, // 2nd order Butterworth band-stop filter
    kLWRLPF2,    // 2nd order Linkwitz-Riley low-pass filter
    kLWRHPF2,    // 2nd order Linkwitz-Riley high-pass filter
    kAPF1,       // 1st order APF filter
    kAPF2,       // 2nd order APF filter
    kLowShelf,   // 1sr order low shelving filter
    kHighShelf,  // 1sr order high shelving filter
    kNCQParaEQ,  // 2nd order non-constant Q parametric EQ filter
    kCQParaEQ,   // 2nd order constant Q parametric EQ filter
    kNumFilterType
};

class IIRFilter {
public:
    void prepare(float sample_rate) {
        sample_rate_ = sample_rate;
        updateBiquadCoeffs(params_, sample_rate_);
    }

    float processSample(float in) {
        // dry + wet
        return coeffs_[d0] * in + coeffs_[c0] * biquad_.processSample(in);
    }

    struct IIRFilterParameters {
    public:
        FilterType type{FilterType::kLPF1};
        float fc{100.0f};
        float Q{.707f};
        float boost_or_cut_db{0.0f};

        IIRFilterParameters() = default;
        IIRFilterParameters(FilterType t, float f, float q, float g)
            : type(t), fc(f), Q(q), boost_or_cut_db(g) {}

        bool operator==(const IIRFilterParameters &p) const {
            return p.type == type &&
                   approximatelyEqual(p.fc, fc, FLT_EPSILON) &&
                   approximatelyEqual(p.Q, Q, FLT_EPSILON) &&
                   approximatelyEqual(p.boost_or_cut_db, boost_or_cut_db,
                                      FLT_EPSILON);
        }

        bool operator!=(const IIRFilterParameters &p) const {
            return !operator==(p);
        }
    };
    void updateParameters(IIRFilterParameters params) {
        if (params_ != params) {
            updateBiquadCoeffs(params, sample_rate_);
        }

        params_ = params;
    }

    const IIRFilterParameters &getParameters() const {
        return params_;
    }

    const Biquad &getBiquad() const {
        return biquad_;
    }

    static FilterCoeffs
    calcFilterCoefficients(const IIRFilterParameters &iir_param,
                           float sample_rate) {
        FilterCoeffs coeffs{{
            0.0f,
        }};

        coeffs[c0] = 1.0f;
        coeffs[d0] = 0.0f;

        const float nyquist = sample_rate / 2;
        const float min_fc = 20;
        const float max_fc = nyquist * 0.93f;

        const auto type = iir_param.type;
        const auto fc = std::clamp(iir_param.fc, min_fc, max_fc);

        const auto Q = iir_param.Q;
        const auto boost_cut_db = iir_param.boost_or_cut_db;

        if (type == FilterType::kLPF1) {
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto gamma = cos(theta_c) / (1 + sin(theta_c));

            coeffs[a0] = (1 - gamma) / 2;
            coeffs[a1] = (1 - gamma) / 2;
            coeffs[a2] = 0.0f;
            coeffs[b1] = -gamma;
            coeffs[b2] = 0.0f;

        } else if (type == FilterType::kHPF1) {
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto gamma = cos(theta_c) / (1 + sin(theta_c));

            coeffs[a0] = (1 + gamma) / 2;
            coeffs[a1] = -(1 + gamma) / 2;
            coeffs[a2] = 0.0f;
            coeffs[b1] = -gamma;
            coeffs[b2] = 0.0f;

        } else if (type == FilterType::kLPF2) {
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto d = 1 / Q;
            const auto beta =
                0.5 * (1 - d / 2 * sin(theta_c)) / (1 + d / 2 * sin(theta_c));
            const auto gama = (0.5f + beta) * cos(theta_c);

            coeffs[a0] = (0.5 + beta - gama) / 2.0f;
            coeffs[a1] = (0.5 + beta - gama);
            coeffs[a2] = (0.5 + beta - gama) / 2.0f;
            coeffs[b1] = -2 * gama;
            coeffs[b2] = 2 * beta;

        } else if (type == FilterType::kHPF2) {
            const auto theta_c = 2.0f * M_PI * fc / sample_rate;
            const auto d = 1 / Q;
            const auto beta =
                0.5 * (1 - d / 2 * sin(theta_c)) / (1 + d / 2 * sin(theta_c));
            const auto gama = (0.5f + beta) * cos(theta_c);

            coeffs[a0] = (0.5 + beta + gama) / 2.0f;
            coeffs[a1] = -(0.5 + beta + gama);
            coeffs[a2] = (0.5 + beta + gama) / 2.0f;
            coeffs[b1] = -2 * gama;
            coeffs[b2] = 2 * beta;

        } else if (type == FilterType::kBPF2) {
            const auto K = tan(M_PI * fc / sample_rate);
            const auto delta = K * K * Q + K + Q;

            coeffs[a0] = K / delta;
            coeffs[a1] = 0.0f;
            coeffs[a2] = -K / delta;
            coeffs[b1] = 2 * Q * (K * K - 1) / delta;
            coeffs[b2] = (K * K * Q - K + Q) / delta;

        } else if (type == FilterType::kBSF2) {
            const auto K = tan(M_PI * fc / sample_rate);
            const auto delta = K * K * Q + K + Q;

            coeffs[a0] = Q * (K * K + 1) / delta;
            coeffs[a1] = 2 * Q * (K * K - 1) / delta;
            coeffs[a2] = Q * (K * K + 1) / delta;
            coeffs[b1] = 2 * Q * (K * K - 1) / delta;
            coeffs[b2] = (K * K * Q - K + Q) / delta;

        } else if (type == FilterType::kButterLPF2) {
            const auto C = 1.0f / (tan(M_PI * fc / sample_rate));

            coeffs[a0] = 1.0f / (1 + M_SQRT2 * C + C * C);
            coeffs[a1] = 2 * coeffs[a0];
            coeffs[a2] = coeffs[a0];
            coeffs[b1] = coeffs[a1] * (1 - C * C);
            coeffs[b2] = coeffs[a0] * (1 - M_SQRT2 * C + C * C);

        } else if (type == FilterType::kButterHPF2) {
            const auto C = 1.0f / (tan(M_PI * fc / sample_rate));

            coeffs[a0] = 1.0f / (1 + M_SQRT2 * C + C * C);
            coeffs[a1] = -2 * coeffs[a0];
            coeffs[a2] = coeffs[a0];
            coeffs[b1] = coeffs[a1] * (C * C - 1);
            coeffs[b2] = coeffs[a0] * (1 - M_SQRT2 * C + C * C);

        } else if (type == FilterType::kButterBPF2) {
            const auto BW = fc / Q;
            const auto C = 1.0f / (tan(M_PI * fc * BW / sample_rate));
            const auto D = 2.0f * cos(2 * M_PI * fc / sample_rate);

            coeffs[a0] = 1.0f / (1 + C);
            coeffs[a1] = 0.0f;
            coeffs[a2] = -coeffs[a0];
            coeffs[b1] = -coeffs[a0] * C * D;
            coeffs[b2] = coeffs[a0] * (C - 1);

        } else if (type == FilterType::kButterBSF2) {
            const auto BW = fc / Q;
            const auto C = 1.0f / (tan(M_PI * fc * BW / sample_rate));
            const auto D = 2.0f * cos(2 * M_PI * fc / sample_rate);

            coeffs[a0] = 1.0f / (1 + C);
            coeffs[a1] = -coeffs[a0] * D;
            coeffs[a2] = coeffs[a0];
            coeffs[b1] = -coeffs[a0] * D;
            coeffs[b2] = coeffs[a0] * (1 - C);

        } else if (type == FilterType::kLWRLPF2) {
            const auto theta_c = M_PI * fc / sample_rate;
            const auto omega_c = M_PI * fc;
            const auto k = omega_c / tan(theta_c);
            const auto delta = k * k + omega_c * omega_c + 2 * k * omega_c;

            const auto b1_num = -2 * k * k + 2 * omega_c * omega_c;
            const auto b2_num = -2 * k * omega_c + k * k + omega_c * omega_c;

            coeffs[a0] = omega_c * omega_c / delta;
            coeffs[a1] = 2 * coeffs[a0];
            coeffs[a2] = coeffs[a0];
            coeffs[b1] = b1_num / delta;
            coeffs[b2] = b2_num / delta;

        } else if (type == FilterType::kLWRHPF2) {
            const auto theta_c = M_PI * fc / sample_rate;
            const auto omega_c = M_PI * fc;
            const auto k = omega_c / tan(theta_c);
            const auto delta = k * k + omega_c * omega_c + 2 * k * omega_c;

            const auto b1_num = -2 * k * k + 2 * omega_c * omega_c;
            const auto b2_num = -2 * k * omega_c + k * k + omega_c * omega_c;

            coeffs[a0] = k * k / delta;
            coeffs[a1] = -2 * coeffs[a0];
            coeffs[a2] = coeffs[a0];
            coeffs[b1] = b1_num / delta;
            coeffs[b2] = b2_num / delta;

        } else if (type == FilterType::kAPF1) {
            const auto alpha_num = tan(M_PI * fc / sample_rate) - 1;
            const auto alpha_dec = tan(M_PI * fc / sample_rate) + 1;
            const auto alpha = alpha_num / alpha_dec;

            coeffs[a0] = alpha;
            coeffs[a1] = 1.0f;
            coeffs[a2] = 0.0f;
            coeffs[b1] = alpha;
            coeffs[b2] = 0.0f;
        } else if (type == FilterType::kAPF2) {
            const auto BW = fc / Q;
            const auto alpha_num = tan(BW * M_PI / sample_rate) - 1;
            const auto alpha_den = tan(BW * M_PI / sample_rate) + 1;
            const auto alpha = alpha_num / alpha_den;
            const auto beta = -cos(2 * M_PI * fc / sample_rate);

            coeffs[a0] = -alpha;
            coeffs[a1] = beta * (1 - alpha);
            coeffs[a2] = 1.0;
            coeffs[b1] = coeffs[a1];
            coeffs[b2] = -alpha;
        } else if (type == FilterType::kLowShelf) {
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);
            const auto beta = 4.0 / (1 + mu);
            const auto delta = beta * tan(theta_c / 2);
            const auto gamma = (1 - delta) / (1 + delta);

            coeffs[a0] = (1 - gamma) / 2;
            coeffs[a1] = coeffs[a0];
            coeffs[a2] = 0.0f;
            coeffs[b1] = -gamma;
            coeffs[b2] = 0.0f;
            coeffs[c0] = mu - 1.0f;
            coeffs[d0] = 1.0f;
        } else if (type == FilterType::kHighShelf) {
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);
            const auto beta = (1 + mu) / 4.0;
            const auto delta = beta * tan(theta_c / 2);
            const auto gamma = (1 - delta) / (1 + delta);

            coeffs[a0] = (1 + gamma) / 2;
            coeffs[a1] = -coeffs[a0];
            coeffs[a2] = 0.0f;
            coeffs[b1] = -gamma;
            coeffs[b2] = 0.0f;
            coeffs[c0] = mu - 1.0f;
            coeffs[d0] = 1.0f;
        } else if (type == FilterType::kNCQParaEQ) {
            const auto theta_c = 2 * M_PI * fc / sample_rate;
            const auto mu = pow(10, boost_cut_db / 20.0f);

            auto tan_arg = theta_c / (2.0 * Q);
            if (tan_arg >= 0.95 * M_PI_2) {
                tan_arg = 0.95 * M_PI_2;
            }
            const auto zeta = 4.0 / (1.0 + mu);
            const auto beta_num = 1.0 - zeta * tan(tan_arg);
            const auto beta_den = 1.0 + zeta * tan(tan_arg);
            const auto beta = 0.5 * beta_num / beta_den;
            const auto gamma = (0.5 + beta) * cos(theta_c);

            coeffs[a0] = 0.5 - beta;
            coeffs[a1] = 0.0f;
            coeffs[a2] = -coeffs[a0];
            coeffs[b1] = -2 * gamma;
            coeffs[b2] = 2 * beta;
            coeffs[c0] = mu - 1.0f;
            coeffs[d0] = 1.0f;
        } else if (type == FilterType::kCQParaEQ) {
            // --- see book for formulae
            const auto K = tan(M_PI * fc / sample_rate);
            const auto Vo = pow(10.0, boost_cut_db / 20.0);
            const bool bBoost = boost_cut_db >= 0;

            const auto d0 = 1.0 + (1.0 / Q) * K + K * K;
            const auto e0 = 1.0 + (1.0 / (Vo * Q)) * K + K * K;
            const auto alpha = 1.0 + (Vo / Q) * K + K * K;
            const auto beta = 2.0 * (K * K - 1.0);
            const auto gamma = 1.0 - (Vo / Q) * K + K * K;
            const auto delta = 1.0 - (1.0 / Q) * K + K * K;
            const auto eta = 1.0 - (1.0 / (Vo * Q)) * K + K * K;

            coeffs[a0] = bBoost ? alpha / d0 : d0 / e0;
            coeffs[a1] = bBoost ? beta / d0 : beta / e0;
            coeffs[a2] = bBoost ? gamma / d0 : delta / e0;
            coeffs[b1] = bBoost ? beta / d0 : beta / e0;
            coeffs[b2] = bBoost ? delta / d0 : eta / e0;
        }

        return coeffs;
    }

private:
    void updateBiquadCoeffs(const IIRFilterParameters &params,
                            float sample_rate) {
        coeffs_ = calcFilterCoefficients(params, sample_rate_);
        biquad_.updateCoeffs(coeffs_);
    }
    IIRFilterParameters params_;
    Biquad biquad_;
    FilterCoeffs coeffs_;
    float sample_rate_{0.0f};
};
} // namespace libaa

#endif // LIBAA_AA_IIR_FILTER_H
