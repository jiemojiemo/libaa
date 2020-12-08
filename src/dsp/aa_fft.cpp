//
// Created by william on 2019/12/6.
//

#include "kiss_fftr.h"
#include "dsp/aa_fft.h"

namespace libaa
{
class FFT::Impl
{
public:
    kiss_fftr_cfg forward_cfg_{nullptr};
    kiss_fftr_cfg inverse_cfg_{nullptr};
};

FFT::FFT(size_t nfft)
    : nfft_(nfft),
      impl_(std::make_shared<Impl>())
{
    impl_->forward_cfg_ = kiss_fftr_alloc(nfft_, 0, 0, 0);
    impl_->inverse_cfg_ = kiss_fftr_alloc(nfft_, 1, 0, 0);
}

FFT::~FFT()
{
    kiss_fftr_free(impl_->forward_cfg_);
    kiss_fftr_free(impl_->inverse_cfg_);
}

void FFT::forward(const float* time_in, std::complex<float>* freq_out)
{
    kiss_fftr(impl_->forward_cfg_, time_in, reinterpret_cast<kiss_fft_cpx*>(freq_out));
}

void FFT::inverse(const std::complex<float>* freq_in, float* time_out)
{
    kiss_fftri(impl_->inverse_cfg_, reinterpret_cast<const kiss_fft_cpx*>(freq_in), time_out);
}

std::vector<float> FFT::fftconvolve(const float* x, size_t x_len, const float* y, size_t y_len)
{
    const size_t output_size = std::max(x_len, y_len);

    std::vector<float> padding_x(output_size);
    std::vector<float> padding_y(output_size);
    for(size_t i = 0; i < x_len; ++i) { padding_x[i] = x[i]; }
    for(size_t i = 0; i < y_len; ++i) { padding_y[i] = y[i]; }

    std::vector<std::complex<float>> X(output_size/2 + 1);
    std::vector<std::complex<float>> Y(output_size/2 + 1);
    FFT fft(output_size);
    fft.forward(padding_x.data(), X.data());
    fft.forward(padding_y.data(), Y.data());

    std::vector<std::complex<float>> complex_mul_result(output_size/2 + 1);
    for(size_t i = 0; i < output_size/2 + 1; ++i)
    {
        complex_mul_result[i] = X[i] * Y[i];
    }

    std::vector<float> conv_result(output_size);
    fft.inverse(complex_mul_result.data(), conv_result.data());

    for(size_t i = 0; i < output_size; ++i)
    {
        conv_result[i] /= float(output_size);
    }

    return conv_result;
}
}