//
// Created by william on 2019/12/24.
//

#pragma once
#include "aa_options.h"
#include <Eigen/Core>
#include <iostream>
using namespace std;

namespace libaa {
using namespace Eigen;
class Distance {
public:
    /**
     * Calculating the distance matrix according the distance type.
     *
     * Returns empty matrix if distance type is invalid @see DistanceType
     *
     * @param X input feature sequence, X.rows() is the dimension of feature
     * that must equals the Y.rows()
     * @param Y input feature sequence
     * @param dist_type distance type @see DistanceType
     */
    static ArrayXXf calcDistance(const ArrayXXf &X, const ArrayXXf &Y,
                                 DistanceType dist_type);

    /**
     * Calculating euclidean distance matrix
     */
    static ArrayXXf calcEuclideanDistance(const ArrayXXf &X, const ArrayXXf &Y);

    static ArrayXXf calcCosineDistance(const ArrayXXf &X, const ArrayXXf &Y);
};
} // namespace libaa