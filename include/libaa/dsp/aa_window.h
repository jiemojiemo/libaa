//
// Created by william on 2019/12/12.
//
#include "aa_options.h"
#include <vector>

namespace libaa {

class Window {
public:
    /**
     * Returns a window of a given length and type.
     */
    static std::vector<float> getWindow(WindowType win_type, size_t win_size,
                                        bool symmetry = true);

    /**
     * Returns a overlap window sum.
     *
     * ISTFT uses overlap window sum to recover signal.
     */
    static std::vector<float> windowSum(WindowType win_type, size_t n_frames,
                                        size_t win_size, size_t hop_size);
};

} // namespace libaa
