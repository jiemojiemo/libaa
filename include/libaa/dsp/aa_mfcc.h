//
// Created by william on 2019/12/15.
//

#pragma once
#include "aa_options.h"
#include "aa_stft.h"
#include <Eigen/Core>

namespace libaa {
using namespace Eigen;
class MFCC {
public:
    static Eigen::ArrayXXf mfcc(const float *data, size_t data_len,
                                int sample_rate, Options opts);

    static Eigen::ArrayXXf spectrogram(const float *data, size_t data_len,
                                       Options opts);

    static Eigen::ArrayXXf melspectrogram(const ArrayXXf &S,
                                          const Options &opts, int sample_rate);

    static ArrayXXf melBanks(const ArrayXXf &S, const Options &opts,
                             int sample_rate);

    static ArrayXXf dctBases(int n_mfcc, int n_mels);
};

} // namespace libaa
