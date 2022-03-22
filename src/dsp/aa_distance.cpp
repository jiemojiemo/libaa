//
// Created by william on 2019/12/24.
//

#include "libaa/dsp/aa_distance.h"

namespace libaa {
ArrayXXf Distance::calcDistance(const ArrayXXf &X, const ArrayXXf &Y,
                                DistanceType dist_type) {
    if (dist_type == DistanceType::kEuclidean) {
        return calcEuclideanDistance(X, Y);
    } else if (dist_type == DistanceType::kCosine) {
        return calcCosineDistance(X, Y);
    }

    return ArrayXXf();
}

ArrayXXf Distance::calcEuclideanDistance(const ArrayXXf &X, const ArrayXXf &Y) {
    // Eucilean Distance
    ArrayXXf D = ArrayXXf::Zero(X.cols(), Y.cols());
    for (int i = 0; i < X.cols(); ++i) {
        D.row(i) = (Y.colwise() - X.col(i)).square().colwise().sum().sqrt();
    }
    return D;
}

ArrayXXf Distance::calcCosineDistance(const ArrayXXf &X, const ArrayXXf &Y) {
    ArrayXXf D = ArrayXXf::Zero(X.cols(), Y.cols());
    ArrayXf norm_y = Y.square().colwise().sum().sqrt();
    ArrayXf norm_x = X.square().colwise().sum().sqrt();
    for (int i = 0; i < X.cols(); ++i) {
        ArrayXf dot_sum = (Y.colwise() * X.col(i)).colwise().sum();
        D.row(i) = 1.0f - dot_sum / (norm_y * norm_x(i));
    }

    return D;
}

} // namespace libaa