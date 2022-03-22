//
// Created by william on 2019/12/26.
//

#pragma once
#include "aa_distance.h"
#include <vector>

namespace libaa {
class DTW {
public:
    using Path = std::vector<std::pair<int, int>>;
    using WindowIndex = std::vector<std::pair<int, int>>;

    /**
     * Returns the distance and path of distance matrix.
     *
     * @param dist_matrix distance matrix that you get from Distance @see
     * Distance
     * @return distance and path
     */
    static std::tuple<float, Path> dtw(const ArrayXXf &dist_matrix);

    /**
     * Calculating distance and path by using dynamic programming.
     *
     * @param dist_matrix distance matrix that you get from Distance @see
     * Distance
     * @param window the search window
     * @return distance and path
     */
    static std::tuple<float, Path> dynamicFindPath(const ArrayXXf &dist_matrix,
                                                   const WindowIndex &window);

    /**
     * Returns the search window
     *
     * @param len_x number of search window rows
     * @param len_y number of search window columns
     * @return search window
     */
    static WindowIndex getWindowIndex(int len_x, int len_y);
};
} // namespace libaa
