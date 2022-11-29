//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_LOUDNESS_EBUR128_EXTRACTOR_H
#define LIBAA_AA_LOUDNESS_EBUR128_EXTRACTOR_H

#pragma once
#include "libaa/mir/aa_ebur128_utilities.h"
#include "libaa/mir/aa_extractor_base.h"

namespace libaa::MIR {
class LoudnessEBUR128Extractor : public ExtractorBase {
public:
    LoudnessEBUR128Extractor();
    ~LoudnessEBUR128Extractor() override;

    std::string getName() const override;
    void configure() override;
    FeatureSet getFrameFeatures() override;
    FeatureSet getRemainingFeatures() override;

    void process(const float *const *input_buffers, size_t samples, bool is_final) override;

    // -- testing --
    const void *getEBURState() const;

private:
    void cleanup();

    void unwrapToInterleave(const float *const *input_buffers, size_t samples);

    void saveFrameFeatures();

    void getOverallFeatures();

    Ebur128Utilities util_;
    ebur128_state *st_{nullptr};
    std::vector<float> interleave_data_;
    std::vector<float> momentary_;
    std::vector<float> shortterm_;
    std::vector<float> integrated_loudness_;
    std::vector<float> range_;
    std::vector<float> true_peak_;
};
} // namespace libaa::MIR

#endif // LIBAA_AA_LOUDNESS_EBUR128_EXTRACTOR_H
