//
// Created by william on 2019/12/12.
//

#pragma once
#include <cstdlib>

namespace libaa {

enum class DistanceType { kEuclidean = 0,
                          kCosine };

enum class WindowType {
    kRectangle = 0,
    kHann,
    kHamming,
    kTriangular,
    kGauss,
    kBlackmanHarris
};

class Options {
public:
    size_t hop_size{256};
    size_t win_size{1024};
    WindowType win_type{WindowType::kHann};

    // mfcc options
    float fmin{50};
    float fmax{3700};
    size_t n_mfcc{20};
    size_t n_mels{88};
    bool norm{true};
};

} // namespace libaa
