//
// Created by william on 2019/12/26.
//

#include "libaa/dsp/aa_dtw.h"

namespace libaa {
std::tuple<float, DTW::Path> DTW::dtw(const ArrayXXf &dist_matrix) {
    const int len_x = dist_matrix.rows();
    const int len_y = dist_matrix.cols();

    WindowIndex window = getWindowIndex(len_x, len_y);

    return dynamicFindPath(dist_matrix, window);
}

std::tuple<float, DTW::Path> DTW::dynamicFindPath(const ArrayXXf &dist_matrix,
                                                  const WindowIndex &window) {
    const int len_x = dist_matrix.rows();
    const int len_y = dist_matrix.cols();
    const float inf = std::numeric_limits<float>::max();

    Eigen::MatrixXf D = Eigen::MatrixXf::Constant(len_x + 1, len_y + 1, inf);
    Eigen::MatrixXi X = Eigen::MatrixXi::Zero(len_x + 1, len_y + 1);
    Eigen::MatrixXi Y = Eigen::MatrixXi::Zero(len_x + 1, len_y + 1);
    D(0, 0) = 0.0f;
    X(0, 0) = 0;
    Y(0, 0) = 0;

    for (const auto &idx : window) {
        const int i = idx.first;
        const int j = idx.second;
        const float dt = dist_matrix(i - 1, j - 1);

        float dist[3] = {dt + D(i, j - 1), dt + D(i - 1, j),
                         dt + D(i - 1, j - 1)};

        auto min_iter = std::min_element(dist, dist + 3);
        auto min_index = std::distance(dist, min_iter);

        D(i, j) = *min_iter;

        // record the path
        X(i, j) = (min_index == 0) ? i : i - 1;
        Y(i, j) = (min_index == 1) ? j : j - 1;
    }

    int i = len_x;
    int j = len_y;
    int tmp_i = 0;
    int tmp_j = 0;

    Path path;
    for (; !(i == 0 && j == 0);) {
        path.emplace_back(i - 1, j - 1);
        tmp_i = X(i, j);
        tmp_j = Y(i, j);

        i = tmp_i;
        j = tmp_j;
    }

    std::reverse(path.begin(), path.end());

    return std::make_tuple(D(len_x, len_y), path);
}

DTW::WindowIndex DTW::getWindowIndex(int len_x, int len_y) {
    assert(len_x > 0);
    assert(len_y > 0);

    WindowIndex window;

    for (int i = 1; i <= len_x; ++i) {
        for (int j = 1; j <= len_y; ++j) {
            window.emplace_back(i, j);
        }
    }

    return window;
}
} // namespace libaa