//
// Created by bytedance on 2021/12/1.
//

#ifndef LIBAA_AA_AUDIO_FILTER_UTILITIES_H
#define LIBAA_AA_AUDIO_FILTER_UTILITIES_H
#include "libaa/dsp/aa_biquad_impl.h"

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
    float fc                {0.0f};
    float Q                 {0.0f};
    float boost_or_cut_db   {0.0f};

    bool operator==(const IIRFilterParameter& p) const{
        return p.type == type &&
               p.fc == fc &&
               p.Q == Q &&
               p.boost_or_cut_db == boost_or_cut_db;
    }

    bool operator!=(const IIRFilterParameter& p) const{
        return !operator==(p);
    }
};

class AudioFilterUtilities
{
public:
    static libaa::FilterCoeffs calcFilterCoefficients(const IIRFilterParameter& iir_param, float sample_rate);
};

}

#endif //LIBAA_AA_AUDIO_FILTER_UTILITIES_H
