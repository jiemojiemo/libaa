//
// Created by user on 11/29/22.
//
#include "libaa/mir/aa_streaming_superflux_novelty.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include <limits>
namespace libaa::MIR {
StreamingSuperFluxNovelty::StreamingSuperFluxNovelty() {
    parameters_.pushIntParameter("binWidth", 3, 3, std::numeric_limits<int>::max());
    parameters_.pushIntParameter("frameWidth", 2, 0, std::numeric_limits<int>::max());

    defineInput(in_bands_, "bands");
    defineOutput(out_differences_, "differences");

    super_flux_novelty_ = MIRAlgorithmFactory::create("SuperFluxNovelty");
}
std::string StreamingSuperFluxNovelty::getName() const {
    return "StreamingSuperFluxNovelty";
}
void StreamingSuperFluxNovelty::configure() {
    frame_width_ = getParameter("frameWidth").getInt();
    super_flux_novelty_->setParameter("binWidth", getParameter("binWidth").getInt());
    super_flux_novelty_->setParameter("frameWidth", frame_width_);
    super_flux_novelty_->input("bands").set(ring_bands_);
    super_flux_novelty_->output("differences").set(super_flux_out_);
    super_flux_novelty_->configure();
}
void StreamingSuperFluxNovelty::compute() {
    const auto &in_bands = in_bands_.get();
    auto &diff = out_differences_.get();

    diff = 0.0f;
    ring_bands_.push_back(in_bands);
    if (ring_bands_.size() == (frame_width_ + 1)) {
        super_flux_novelty_->compute();
        diff = super_flux_out_;

        // moving bands forward
        for (auto i = 0u; i < frame_width_; ++i) {
            ring_bands_[i] = ring_bands_[i + 1];
        }
        ring_bands_.resize(frame_width_);
    }
}

// -- testing --
const MIRAlgorithmBase *StreamingSuperFluxNovelty::getSuperFluxNovelty() const {
    return dynamic_cast<MIRAlgorithmBase *>(super_flux_novelty_.get());
}

const std::vector<std::vector<float>> &StreamingSuperFluxNovelty::getRingBands() const {
    return ring_bands_;
}
void StreamingSuperFluxNovelty::reset() {
    ring_bands_.clear();
}

} // namespace libaa::MIR
