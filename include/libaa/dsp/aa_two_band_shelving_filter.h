//
// Created by user on 4/18/22.
//

#ifndef LIBAA_AA_TWO_BAND_SHELVING_FILTER_H
#define LIBAA_AA_TWO_BAND_SHELVING_FILTER_H

#pragma once
#include "libaa/dsp/aa_iir_filter.h"
namespace libaa {
class TwoBandShelvingFilter {
public:
    void prepare(float sample_rate) {
        low_shelving_filter.prepare(sample_rate);
        high_shelving_filter.prepare(sample_rate);
    }

    float processSample(float in) {
        float x = low_shelving_filter.processSample(in);
        return high_shelving_filter.processSample(x);
    }

    struct TwoBandShelvingFilterParameters {
        float low_shelf_fc{0.0f};
        float low_shelf_boost_db{0.0f};
        float high_shelf_fc{0.0f};
        float high_shelf_boost_db{0.0f};
    };
    void updateParameters(TwoBandShelvingFilterParameters params) {
        if (isLowShelvingParameterChanged(params)) {
            updateLowShelvingFilterParameters(params);
        }

        if (isHighShelvingParameterChanged(params)) {
            updateHighShelvingFilterParameters(params);
        }

        params_ = params;
    }

    const TwoBandShelvingFilterParameters &getParameters() {
        return params_;
    }

    const IIRFilter &getLowShelvingFilter() const {
        return low_shelving_filter;
    }

    const IIRFilter &getHighShelvingFilter() const {
        return high_shelving_filter;
    }

private:
    bool isLowShelvingParameterChanged(
        const TwoBandShelvingFilterParameters &params) const {
        return !approximatelyEqual(params_.low_shelf_fc, params.low_shelf_fc,
                                   FLT_EPSILON) ||
               !approximatelyEqual(params_.low_shelf_boost_db,
                                   params.low_shelf_boost_db, FLT_EPSILON);
    }

    bool isHighShelvingParameterChanged(
        const TwoBandShelvingFilterParameters &params) const {
        return !approximatelyEqual(params_.high_shelf_fc, params.high_shelf_fc,
                                   FLT_EPSILON) ||
               !approximatelyEqual(params_.high_shelf_boost_db,
                                   params.high_shelf_boost_db, FLT_EPSILON);
    }

    void updateLowShelvingFilterParameters(
        const TwoBandShelvingFilterParameters &params) {

        IIRFilter::IIRFilterParameters new_iir_param;
        new_iir_param.type = FilterType::kLowShelf;
        new_iir_param.fc = params.low_shelf_fc;
        new_iir_param.boost_or_cut_db = params.low_shelf_boost_db;
        low_shelving_filter.updateParameters(new_iir_param);
    }

    void updateHighShelvingFilterParameters(
        const TwoBandShelvingFilterParameters &params) {
        IIRFilter::IIRFilterParameters new_iir_param;
        new_iir_param.type = FilterType::kHighShelf;
        new_iir_param.fc = params.high_shelf_fc;
        new_iir_param.boost_or_cut_db = params.high_shelf_boost_db;
        high_shelving_filter.updateParameters(new_iir_param);
    }

    TwoBandShelvingFilterParameters params_;
    IIRFilter low_shelving_filter;
    IIRFilter high_shelving_filter;
};
} // namespace libaa

#endif // LIBAA_AA_TWO_BAND_SHELVING_FILTER_H
