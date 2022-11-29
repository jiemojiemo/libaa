//
// Created by user on 11/29/22.
//

#include "libaa/mir/aa_loudness_ebur128_extractor.h"
#include "ebur128.h"

namespace libaa::MIR {
LoudnessEBUR128Extractor::LoudnessEBUR128Extractor() {
}
LoudnessEBUR128Extractor::~LoudnessEBUR128Extractor() {
    cleanup();
}

std::string LoudnessEBUR128Extractor::getName() const {
    return {"LoudnessEBUR128"};
}
void LoudnessEBUR128Extractor::configure() {
    // set max block size to 0 when configure
    // we change the buffer size in processing state if need
    util_.prepare(input_channels_, context_.sample_rate, 0);
}
ExtractorBase::FeatureSet LoudnessEBUR128Extractor::getFrameFeatures() {
    FeatureSet return_features;

    auto push_float_feature = [&return_features](std::vector<float> &values, std::string label, int feature_index) {
        Feature f;
        f.has_timestamp = false;
        f.has_duration = false;
        f.label = std::move(label);
        f.values = values;

        return_features[feature_index].push_back(f);

        values.clear();
    };

    push_float_feature(momentary_, "momentary", 0);
    push_float_feature(shortterm_, "short term loudness", 1);
    push_float_feature(integrated_loudness_, "integrated loudness", 2);
    push_float_feature(range_, "loudness range", 3);
    push_float_feature(true_peak_, "true peak", 4);

    return return_features;
}
ExtractorBase::FeatureSet LoudnessEBUR128Extractor::getRemainingFeatures() {
    return {};
}

void LoudnessEBUR128Extractor::process(const float *const *input_buffers, size_t samples, bool is_final) {
    (void)(is_final);

    if (util_.getInternalInterleaveBuffer().size() < input_channels_ * samples) {
        util_.resizeBuffer(input_channels_, samples);
    }

    util_.process(input_buffers, input_channels_, samples);

    saveFrameFeatures();
}

// -- testing --
const void *LoudnessEBUR128Extractor::getEBURState() const {
    return util_.st;
}

void LoudnessEBUR128Extractor::cleanup() {
    if (st_ != nullptr) {
        ebur128_destroy(&st_);
        st_ = nullptr;
    }
}

void LoudnessEBUR128Extractor::unwrapToInterleave(const float *const *input_buffers, size_t samples) {
    interleave_data_.resize(samples * input_channels_);
    for (auto c = 0u; c < input_channels_; ++c) {
        for (auto i = 0u; i < samples; ++i) {
            interleave_data_[i * input_channels_ + c] = input_buffers[c][i];
        }
    }
}

void LoudnessEBUR128Extractor::saveFrameFeatures() {
    momentary_.push_back(util_.momentary);
    shortterm_.push_back(util_.shorterm_loudness);
    integrated_loudness_.push_back(util_.global_integrated_loudness);
    range_.push_back(util_.range);
    true_peak_.insert(true_peak_.end(), util_.peaks.begin(), util_.peaks.end());
}
} // namespace libaa::MIR