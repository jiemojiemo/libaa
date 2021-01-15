//
// Created by william on 2019/12/11.
//

#include "dsp/aa_window.h"
#include "dsp/aa_stft.h"
#include "dsp/aa_dsp_utilities.h"

namespace libaa
{


Eigen::ArrayXXcf STFT::stft(const float* data, size_t data_len, const Options& opts)
{
    if(!DSPUtilities::isPowerOf2(opts.win_size))
    {
        std::cerr << "stft only support window size of 2^n\n";
        return Eigen::ArrayXXcf();
    }

    const int nfft_out = opts.win_size / 2 + 1;
    const int num_frames = data_len / opts.hop_size;

    Eigen::ArrayXXcf S(nfft_out, num_frames);
    auto* result_data = S.data();

    Eigen::ArrayXf cur_frame(opts.win_size);

    FFT fft(opts.win_size);
    size_t idx = 0;
    std::vector<float> w = Window::getWindow(opts.win_type, opts.win_size);
    Eigen::Map<Eigen::ArrayXf> window(w.data(), w.size());

    for(;idx < data_len - opts.hop_size;)
    {
        std::copy(data + idx,
                  data + idx + opts.win_size, cur_frame.data());

        cur_frame *= window;

        fft.forward(cur_frame.data(), result_data);

        result_data += nfft_out;
        idx += opts.hop_size;
    }

    return S;
}

Eigen::ArrayXf STFT::istft(const Eigen::MatrixXcf& S, const Options& opts)
{
    // istft
    const size_t istft_out_len = (S.cols() - 1) * opts.hop_size + opts.win_size;
    Eigen::ArrayXf output(istft_out_len);

    FFT fft(opts.win_size);
    auto* freq_data = S.data();
    Eigen::ArrayXf time_data(opts.win_size);

    const int n_frames = S.cols();
    for(int i = 0; i < n_frames; ++i)
    {
        fft.inverse(freq_data, time_data.data());

        int overlap_add_i = i * opts.hop_size;

        // normalize the ifft result
        time_data /= opts.win_size;

        output.segment(overlap_add_i, opts.win_size) += time_data;

        freq_data += S.rows();
    }

    auto w_sum = Window::windowSum(opts.win_type, n_frames, opts.win_size, opts.hop_size);
    Eigen::Map<Eigen::ArrayXf> window_sum(w_sum.data(), w_sum.size());

    window_sum = (window_sum < 1e-6).select(1, window_sum);
    output /= window_sum;

    return output;
}


}
