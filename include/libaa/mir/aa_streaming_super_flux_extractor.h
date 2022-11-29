//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_STREAMING_SUPER_FLUX_EXTRACTOR_H
#define LIBAA_AA_STREAMING_SUPER_FLUX_EXTRACTOR_H

#pragma once
#include "libaa/core/aa_processing_context.h"
#include "libaa/mir/aa_extractor_base.h"

namespace libaa::MIR {

class MIRAlgorithmBase;
class IMIRAlgorithm;

class SuperFluxExtractor : public ExtractorBase {
public:
    SuperFluxExtractor();
    ~SuperFluxExtractor() override = default;
    std::string getName() const override;
    FeatureSet getFrameFeatures() override;
    FeatureSet getRemainingFeatures() override;
    void configure() override;
    void processBlock(std::vector<std::vector<float>> &input_buffers, ProcessingContext &context) override;

    // -- testing --
    const MIRAlgorithmBase *getWindowing() const;
    const MIRAlgorithmBase *getSpectrum() const;
    const MIRAlgorithmBase *getTriangularBands() const;
    const MIRAlgorithmBase *getStreamingSuperFluxNovelty() const;
    const MIRAlgorithmBase *getStreamingSuperFluxPeaks() const;

private:
    void synchronizeParameters();
    void configureAlgorithmInputsAndOutputs();
    void configureAlgorithmParameters();

    void configureAlgorithm();

    size_t frame_size_{0};
    size_t hop_size_{0};
    float combine_{0.0f};
    float threshold_{0.0f};
    float ratio_threshold_{0.0f};
    std::unique_ptr<MIR::IMIRAlgorithm> window;
    std::unique_ptr<MIR::IMIRAlgorithm> spectrum;
    std::unique_ptr<MIR::IMIRAlgorithm> tri;
    std::unique_ptr<MIR::IMIRAlgorithm> super_flux_novelty;
    std::unique_ptr<MIR::IMIRAlgorithm> super_flux_peaks;
    std::vector<float> windowed_buffer_;
    std::vector<float> spectrum_buffer_;
    std::vector<float> bands_;
    std::vector<float> peaks_;
    std::vector<float> all_peaks_;
    float novelty_;
};
} // namespace libaa::MIR

#endif // LIBAA_AA_STREAMING_SUPER_FLUX_EXTRACTOR_H
