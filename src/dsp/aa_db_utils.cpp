
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/dsp/aa_db_utils.h"
#include <cmath>
namespace libaa
{
float db_to_scale(float db)
{
    return std::powf(10.0f, db/20.0f);
}
}