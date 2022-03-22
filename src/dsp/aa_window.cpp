//
// Created by william on 2019/12/12.
//

#include "libaa/dsp/aa_window.h"
#include <functional>
#include <math.h>

namespace libaa {

float hammingAtPoint(int point, int numSamples) {
    return 0.54f -
           0.46f * cosf((2.0f * M_PI * point) / ((float)numSamples - 1.0f));
}

float triangularAtPoint(int point, int numSamples) {
    return (2.0f / numSamples) *
           ((numSamples * 0.5f) - fabs(point - (numSamples - 1.0f) * 0.5f));
}

float gaussAtPoint(int point, int numSamples) {

    float bellW = 0.4f;
    return powf(M_E, -0.5f * powf((point - (numSamples - 1) * 0.5f) /
                                      (bellW * (numSamples - 1) * 0.5f),
                                  2.0f));
}

float blackmanHarrisAtPoint(int point, int numSamples) {

    return 0.35875f - 0.48829f * cosf(2.0f * M_PI * point / (numSamples - 1)) +
           0.14128f * cosf(4.0f * M_PI * point / (numSamples - 1)) -
           0.01168f * cosf(6.0f * M_PI * point / (numSamples - 1));
}

float hanningAtPoint(float point, int numSamples) {
    return 0.5f *
           (1.0f - cosf((2.0f * M_PI * point) / ((float)numSamples - 1.0f)));
}

std::vector<float> Window::windowSum(WindowType win_type, size_t n_frames,
                                     size_t win_size, size_t hop_size) {
    size_t n = (n_frames - 1) * hop_size + win_size;
    std::vector<float> x(n, 0.0f);

    std::vector<float> w = Window::getWindow(win_type, win_size);

    for (size_t i = 0; i < n_frames; ++i) {
        const int overlap_add_i = i * hop_size;

        for (size_t j = 0; j < win_size; ++j) {
            x[overlap_add_i + j] += w[j];
        }
    }

    return x;
}

std::vector<float> Window::getWindow(WindowType win_type, size_t win_size,
                                     bool symmetry) {
    std::function<float(int, int)> win_func = hammingAtPoint;

    if (win_type == WindowType::kRectangle) {
        return std::vector<float>(win_size, 1.0f);
    } else if (win_type == WindowType::kHann) {
        win_func = hanningAtPoint;
    } else if (win_type == WindowType::kHamming) {
        win_func = hammingAtPoint;
    } else if (win_type == WindowType::kTriangular) {
        win_func = triangularAtPoint;
    } else if (win_type == WindowType::kGauss) {
        win_func = gaussAtPoint;
    } else if (win_type == WindowType::kBlackmanHarris) {
        win_func = blackmanHarrisAtPoint;
    }

    std::vector<float> window(win_size);
    size_t num_sample = symmetry ? win_size : win_size + 1;

    for (size_t i = 0; i < win_size; ++i) {
        window[i] = win_func(i, num_sample);
    }

    return window;
}

} // namespace libaa