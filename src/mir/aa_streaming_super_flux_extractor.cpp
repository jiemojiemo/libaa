//
// Created by user on 11/29/22.
//
#include "libaa/mir/aa_streaming_super_flux_extractor.h"
#include "libaa/mir/aa_mir_algorithm.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"

using namespace std;
namespace libaa::MIR {
SuperFluxExtractor::SuperFluxExtractor() {
    parameters_.pushFloatParameter("combine", 20, 0, std::numeric_limits<float>::max());
    parameters_.pushIntParameter("frameSize", 2048, 0, std::numeric_limits<int>::max());
    parameters_.pushIntParameter("hopSize", 256, 0, std::numeric_limits<int>::max());
    parameters_.pushFloatParameter("ratioThreshold", 16, 0, std::numeric_limits<float>::max());
    parameters_.pushFloatParameter("threshold", 0.05, 0, std::numeric_limits<float>::max());

    window = MIR::MIRAlgorithmFactory::create("Windowing");
    spectrum = MIR::MIRAlgorithmFactory::create("Spectrum");
    tri = MIR::MIRAlgorithmFactory::create("TriangularBands");
    super_flux_novelty = MIR::MIRAlgorithmFactory::create("StreamingSuperFluxNovelty");
    super_flux_peaks = MIR::MIRAlgorithmFactory::create("StreamingSuperFluxPeaks");
}
std::string SuperFluxExtractor::getName() const {
    return "SuperFluxExtractor";
}
ExtractorBase::FeatureSet SuperFluxExtractor::getFrameFeatures() {

    FeatureSet return_features;
    for (const auto &p : all_peaks_) {
        Feature onset;
        onset.has_timestamp = true;
        onset.timestamp = p;
        return_features[0].push_back(onset);
    }

    all_peaks_.clear();

    return return_features;
}
ExtractorBase::FeatureSet SuperFluxExtractor::getRemainingFeatures() {
    return libaa::MIR::IExtractor::FeatureSet();
}
void SuperFluxExtractor::configure() {
    synchronizeParameters();

    configureAlgorithm();

    if (frame_size_ != block_size_ || hop_size_ != step_size_) {
        initializeBuffers(input_channels_, hop_size_, frame_size_);
    }
}
void SuperFluxExtractor::processBlock(std::vector<std::vector<float>> &input_buffers, ProcessingContext &context) {
    (void)(context);
    window->input("frame").set(input_buffers[0]); // only use left channel data
    window->compute();

    spectrum->compute();
    tri->compute();
    super_flux_novelty->compute();
    super_flux_peaks->compute();

    all_peaks_.insert(all_peaks_.end(), peaks_.begin(), peaks_.end());
}

// -- testing --
const MIRAlgorithmBase *SuperFluxExtractor::getWindowing() const {
    return dynamic_cast<MIRAlgorithmBase *>(window.get());
}

const MIRAlgorithmBase *SuperFluxExtractor::getSpectrum() const {
    return dynamic_cast<MIRAlgorithmBase *>(spectrum.get());
}

const MIRAlgorithmBase *SuperFluxExtractor::getTriangularBands() const {
    return dynamic_cast<MIRAlgorithmBase *>(tri.get());
}

const MIRAlgorithmBase *SuperFluxExtractor::getStreamingSuperFluxNovelty() const {
    return dynamic_cast<MIRAlgorithmBase *>(super_flux_novelty.get());
}

const MIRAlgorithmBase *SuperFluxExtractor::getStreamingSuperFluxPeaks() const {
    return dynamic_cast<MIRAlgorithmBase *>(super_flux_peaks.get());
}

void SuperFluxExtractor::synchronizeParameters() {
    frame_size_ = getParameter("frameSize").getInt();
    hop_size_ = getParameter("hopSize").getInt();
    combine_ = getParameter("combine").getPlainValue();
    threshold_ = getParameter("threshold").getPlainValue();
    ratio_threshold_ = getParameter("ratioThreshold").getPlainValue();
}
void SuperFluxExtractor::configureAlgorithmInputsAndOutputs() {
    windowed_buffer_.resize(frame_size_, 0.0f);
    window->output("frame").set(windowed_buffer_);

    spectrum->input("frame").set(windowed_buffer_);
    spectrum->output("spectrum").set(spectrum_buffer_);

    tri->input("spectrum").set(spectrum_buffer_);
    tri->output("bands").set(bands_);

    super_flux_novelty->input("bands").set(bands_);
    super_flux_novelty->output("differences").set(novelty_);

    super_flux_peaks->input("novelty").set(novelty_);
    super_flux_peaks->output("peaks").set(peaks_);
}
void SuperFluxExtractor::configureAlgorithmParameters() {
    window->setParameter("size", frame_size_);
    tri->setParameter("inputSize", frame_size_ / 2 + 1); // the output of spectrum is half fft size
    spectrum->setParameter("size", frame_size_);

    static const int kBinWidth = 8;
    super_flux_novelty->setParameter("binWidth", kBinWidth);

    super_flux_peaks->setParameter("combine", combine_);
    super_flux_peaks->setParameter("ratioThreshold", ratio_threshold_);
    super_flux_peaks->setParameter("threshold", threshold_);
    super_flux_peaks->setParameter("frameRate", context_.sample_rate / hop_size_);
}

void SuperFluxExtractor::configureAlgorithm() {
    configureAlgorithmInputsAndOutputs();
    configureAlgorithmParameters();

    window->configure();
    spectrum->configure();
    tri->configure();
    super_flux_novelty->configure();
    super_flux_peaks->configure();
}
} // namespace libaa::MIR
