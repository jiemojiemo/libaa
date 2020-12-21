
//
// Created by William.Hua on 2020/12/21.
//

#pragma once
#include "audio_effect/aa_audio_effect_processor.h"
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

class IIRFilter : public AudioEffectProcessor
{
public:
    static FilterCoeffs calcFilterCoeffs(const IIRFilterParameter& iir_param, float sample_rate);

    IIRFilter();

    std::string getName() const override{
        return std::string("IIRFilter");
    }

    void prepareToPlay(double sample_rate, int max_block_size) override;
    void reset() override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float> &buffer) override;

    int setParameter(IIRFilterParameter param);
private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};
}
