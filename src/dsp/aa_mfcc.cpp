//
// Created by william on 2019/12/15.
//

#include "libaa/dsp/aa_mfcc.h"

namespace libaa {
Eigen::ArrayXXf MFCC::mfcc(const float *data, size_t data_len, int sample_rate,
                           Options opts) {
    Eigen::ArrayXXf S = spectrogram(data, data_len, opts);

    ArrayXXf mel_S = melspectrogram(S, opts, sample_rate);

    ArrayXXf bases = dctBases(opts.n_mfcc, opts.n_mels);

    return bases.matrix() * mel_S.matrix();
}

Eigen::ArrayXXf MFCC::spectrogram(const float *data, size_t data_len,
                                  Options opts) {
    Eigen::ArrayXXcf stft_result = STFT::stft(data, data_len, opts);

    ArrayXXf S = stft_result.abs().square();

    return S;
}

Eigen::ArrayXXf MFCC::melspectrogram(const ArrayXXf &S, const Options &opts,
                                     int sample_rate) {
    ArrayXXf mel_banks = melBanks(S, opts, sample_rate);

    return mel_banks.matrix() * S.matrix();
}

ArrayXXf MFCC::melBanks(const ArrayXXf &S, const Options &opts,
                        int sample_rate) {
    auto freq2Mel = [](float freq) {
        return 2595.0f * log10(1.0f + freq / 700.0f);
    };
    auto mel2Freq = [](float mel) {
        return 700.0f * (powf(10, mel / 2595.0f) - 1.0f);
    };

    const float fmin_mel = freq2Mel(opts.fmin);
    const float fmax_mel = freq2Mel(opts.fmax);
    const int n_fft = S.rows();

    // freq points = opts.n_mels + 2
    Eigen::ArrayXf mels =
        ArrayXf::LinSpaced(opts.n_mels + 2, fmin_mel, fmax_mel);
    ArrayXf freqs = mels.unaryExpr(mel2Freq).floor();
    ArrayXi freq_points = ((freqs / float(sample_rate)) * n_fft).cast<int>();

    ArrayXXf mel_banks = ArrayXXf::Zero(opts.n_mels, n_fft);
    int num_weights = 0;
    for (size_t i = 0; i < opts.n_mels; ++i) {
        const int freq = freq_points[i];
        const int freq_next = freq_points[i + 1];
        const int freq_nnext = freq_points[i + 2];

        num_weights = freq_next - freq;
        mel_banks.row(i).segment(freq, num_weights) =
            VectorXf::LinSpaced(num_weights, 0, 1);

        num_weights = freq_nnext - freq_next;
        mel_banks.row(i).segment(freq_next, num_weights) =
            VectorXf::LinSpaced(num_weights, 1, 0);
    }

    if (opts.norm) {
        ArrayXf enorm = freqs.tail(opts.n_mels) - freqs.head(opts.n_mels);
        mel_banks = (mel_banks.colwise() / enorm).eval();
    }
    return mel_banks;
}

ArrayXXf MFCC::dctBases(int n_mfcc, int n_mels) {
    ArrayXXf dct_bases = ArrayXXf::Zero(n_mfcc, n_mels);

    dct_bases.row(0) = ArrayXf::Constant(n_mels, 1.0f / sqrt(n_mels));

    ArrayXf samples =
        ArrayXf::LinSpaced(n_mels, 0, n_mels - 1) * sqrt(2.0f / n_mels);
    for (int i = 1; i < n_mfcc; ++i) {
        dct_bases.row(i) = (samples * float(i)).cos();
    }

    return dct_bases;
}
} // namespace libaa