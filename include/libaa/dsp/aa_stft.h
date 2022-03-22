//
// Created by william on 2019/12/11.
//

#pragma once
#include "aa_fft.h"
#include "aa_options.h"
#include <Eigen/Core>
#include <iostream>
#include <vector>

namespace libaa {

class STFT {
public:
    static Eigen::ArrayXXcf stft(const float *data, size_t data_len,
                                 const Options &opts);
    static Eigen::ArrayXf istft(const Eigen::MatrixXcf &S, const Options &opts);
};

} // namespace libaa
