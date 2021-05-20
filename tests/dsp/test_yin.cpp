//
// Created by william on 2019/12/16.
//

#include "libaa/dsp/aa_yin.h"
#include <gmock/gmock.h>
#include <vector>
#include <Eigen/Core>
using namespace Eigen;
using namespace libaa;

using namespace testing;
using namespace std;


ArrayXf genCosWave(size_t size, float f0, size_t sample_rate)
{
    const float PI = std::atan(1.0)*4;

    ArrayXf n = 2*PI*f0/float(sample_rate) * ArrayXf::LinSpaced(size, 0, size - 1);

    return  n.cos();
}

TEST(AYin, calucateAutocorrelation)
{
    size_t sample_rate = 22050;
    float f0 = 250;
    auto fake_data = genCosWave(10000, f0, sample_rate);

    const float threshold = 0.1;
    size_t block_size = fake_data.size();

    Yin yin(block_size, sample_rate, threshold);

    ASSERT_THAT(yin.getPitch(fake_data.data()), FloatNear(f0, f0*0.01));
}