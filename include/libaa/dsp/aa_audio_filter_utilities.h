//
// Created by bytedance on 2021/12/1.
//

#ifndef LIBAA_AA_AUDIO_FILTER_UTILITIES_H
#define LIBAA_AA_AUDIO_FILTER_UTILITIES_H
#include "libaa/dsp/aa_biquad_impl.h"

namespace libaa {

// class AudioFilterUtilities {
// public:
//     static libaa::FilterCoeffs
//     calcFilterCoefficients(const IIRFilterParameters &iir_param,
//                            float sample_rate);
//
//     void updateCoeffes(FilterType type, float fc, float Q, float
//     boost_or_cut,
//                        float sample_rate) {
//         updateCoeffes(IIRFilterParameters{type, fc, Q, boost_or_cut},
//                       sample_rate);
//     }
//
//     void updateCoeffes(const IIRFilterParameters &iir_param, float
//     sample_rate) {
//         auto coeffs = calcFilterCoefficients(iir_param, sample_rate);
//         for (auto &b : biquad_array) {
//             b.updateCoeffs(coeffs);
//         }
//     }
//
//     std::array<Biquad, 2> biquad_array;
// };

} // namespace libaa

#endif // LIBAA_AA_AUDIO_FILTER_UTILITIES_H
