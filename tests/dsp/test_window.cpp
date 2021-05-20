//
// Created by william on 2019/12/12.
//
#include "libaa/dsp/aa_window.h"
#include <gmock/gmock.h>
#include <Eigen/Core>
#include <vector>
using namespace std;
using namespace testing;
using namespace libaa;

TEST(GetWindow, returnsWindowWithWinSize)
{
    const int win_size = 512;
    std::vector<float> window = Window::getWindow(WindowType::kRectangle, win_size);

    ASSERT_THAT(window.size(), Eq(win_size));
    ASSERT_THAT(window, ContainerEq(vector<float>(win_size, 1.0f)));
}