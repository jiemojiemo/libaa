
//
// Created by William.Hua on 2020/12/21.
//

#include "audio_effect/aa_iir_filter.h"
#include "audio_effect/aa_biquad_filter.h"

namespace libaa
{
FilterCoeffs IIRFilter::calcFilterCoeffs(const IIRFilterParameter& iir_param, float sample_rate)
{
    FilterCoeffs coeffs{0.0f, };

    coeffs[Biquad::c0] = 1.0f;
    coeffs[Biquad::d0] = 0.0f;

    const auto type = iir_param.type;
    const auto fc = iir_param.fc;
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

class IIRFilter::Impl
{
public:
    void reset(){
        biquad_filter_.reset();

        param_.type             = FilterType::kLPF1;
        param_.fc               = 0.0f;
        param_.Q                = 0.707;
        param_.boost_or_cut_db  = 0.0f;
    }

    IIRFilterParameter param_;
    BiquadFilter biquad_filter_;
};

IIRFilter::IIRFilter():
    impl_(std::make_shared<Impl>())
{

}

void IIRFilter::prepareToPlay(double sample_rate, int max_block_size) {
    impl_->biquad_filter_.prepareToPlay(sample_rate, max_block_size);
}
void IIRFilter::reset() {
    impl_->reset();
}
void IIRFilter::releaseResources() {
    impl_->biquad_filter_.releaseResources();
}
void IIRFilter::processBlock(AudioBuffer<float> &buffer) {
    impl_->biquad_filter_.processBlock(buffer);
}
int IIRFilter::setParameter(IIRFilterParameter param) {
    if(param != impl_->param_){
        impl_->param_ = param;
    }

    if(impl_->param_.Q <= 0){
        impl_->param_.Q = .707f;
    }

    auto new_coeffs = calcFilterCoeffs(impl_->param_, getSampleRate());
    impl_->biquad_filter_.setCoefficients(new_coeffs);

    return 0;
}

}