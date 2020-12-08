//
// Created by william on 2019/12/6.
//

#pragma once

#include <complex>
#include <vector>
#include <memory>
namespace libaa
{
class FFT
{
public:
    explicit FFT(size_t nfft);

    ~FFT();

    void forward(const float* time_in, std::complex<float>* freq_out);
    void inverse(const std::complex<float>* freq_in, float* time_out);

    static std::vector<float> fftconvolve(const float* x, size_t x_len, const float* y, size_t y_len);

private:
    size_t nfft_{0};

    class Impl;
    std::shared_ptr<Impl> impl_;
};

}
