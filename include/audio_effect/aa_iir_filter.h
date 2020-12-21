
//
// Created by William.Hua on 2020/12/21.
//

#pragma once
#include "audio_effect/aa_biquad_impl.h"
#include <cmath>
namespace libaa
{

enum class FilterType
{
    kLPF1 = 0,          // 1st order low-pass filter
    kHPF1,              // 1st order high-pass filter
    kLPF2,              // 2nd order low-pass filter
    kHPF2,              // 2nd order high-pass filter
    kBPF2,              // 2nd order band-pass filter
    kBSF2,              // 2nd order band-stop filter
    kButterLPF2,        // 2nd order Butterworth low-pass filter
    kButterHPF2,        // 2nd order Butterworth high-pass filter
    kButterBPF2,        // 2nd order Butterworth band-pass filter
    kButterBSF2,        // 2nd order Butterworth band-stop filter
    kLWRLPF2,           // 2nd order Linkwitz-Riley low-pass filter
    kLWRHPF2,           // 2nd order Linkwitz-Riley high-pass filter
    kAPF1,              // 1st order APF filter
    kAPF2,              // 2nd order APF filter
    kLowShelf,          // 1sr order low shelving filter
    kHighShelf,         // 1sr order high shelving filter
    kNCQParaEQ,         // 2nd order non-constant Q parametric EQ filter
    kCQParaEQ,          // 2nd order constant Q parametric EQ filter
};

class IIRFilterParameter
{
public:
    FilterType type         {FilterType::kLPF1};
    float sample_rate       {0.0f};
    float fc                {0.0f};
    float Q                 {0.0f};
    float boost_or_cut_db   {0.0f};
};

class IIRFilter
{
public:
    static FilterCoeffs calcFilterCoeffs(const IIRFilterParameter& iir_param);
};
}
